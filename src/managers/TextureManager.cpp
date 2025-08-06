#include "TextureManager.h"
#include <stb_image.h>
#include <iostream>
#include <logger.cpp>

std::unordered_map<std::string, GLuint> TextureManager::textureCache;

GLuint TextureManager::loadTexture(const std::string& path) {
    if (textureCache.find(path) != textureCache.end()) {
        return textureCache[path];
    }

    int width, height, channels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

    if (!data) {
		Logger::log(Logger::ERROR, "Failed to load texture: " + path);
        return 0;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    GLenum format = channels == 4 ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    textureCache[path] = textureID;
    return textureID;
}

GLuint TextureManager::getTexture(const std::string& path) {
    auto it = textureCache.find(path);
    return (it != textureCache.end()) ? it->second : loadTexture(path);
}