#include "TextureManager.h"
#include <stb_image.h>
#include <iostream>
#include <logger.cpp>
#include <algorithm>


GLuint TextureManager::uploadToGpu(TextureData& textureData) {
    if (textureData.id == 0) {
        glGenTextures(1, &textureData.id);
    }

    glBindTexture(GL_TEXTURE_2D, textureData.id);

    GLenum format = textureData.channels == 4 ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, textureData.width, textureData.height, 0,
        format, GL_UNSIGNED_BYTE, textureData.pixels.data());

    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    textureData.inGpu = true;
    return textureData.id;
}

GLuint TextureManager::loadTexture(const std::string& path) {
    auto it = textureCache.find(path);

    // Если текстура в кэше и загружена в GPU
    if (it != textureCache.end() && it->second.inGpu) {
        return it->second.id;
    }

    // Если в кэше есть данные, но не в GPU
    if (it != textureCache.end()) {
        return uploadToGpu(it->second);
    }

    // Загрузка нового файла
    TextureData newData;
    unsigned char* data = stbi_load(path.c_str(), &newData.width, &newData.height, &newData.channels, 0);

    if (!data) {
        Logger::log(Logger::ERROR, "Failed to load texture: " + path);
        return 0;
    }

    // Копируем данные в вектор
    size_t dataSize = newData.width * newData.height * newData.channels;
    newData.pixels.assign(data, data + dataSize);
    stbi_image_free(data);

    // Загружаем в GPU
    newData.id = uploadToGpu(newData);

    // Сохраняем в кэш
    textureCache[path] = newData;

    return newData.id;
}


GLuint TextureManager::getTexture(const std::string& path) {
    return loadTexture(path); // Просто обёртка
}

void TextureManager::freeTexture(const std::string& path, bool force) {
    auto it = textureCache.find(path);
    if (it != textureCache.end()) {
        if (it->second.inGpu) {
            glDeleteTextures(1, &it->second.id);
            it->second.inGpu = false;
        }

        if (force) {
            textureCache.erase(it);
        }
    }
}

void TextureManager::unloadFromGpu(const std::string& path) {
    auto it = textureCache.find(path);
    if (it != textureCache.end() && it->second.inGpu) {
        glDeleteTextures(1, &it->second.id);
        it->second.id = 0;
        it->second.inGpu = false;
    }
}

void TextureManager::reloadToGpu(const std::string& path) {
    auto it = textureCache.find(path);
    if (it != textureCache.end() && !it->second.inGpu) {
        uploadToGpu(it->second);
    }
}

size_t TextureManager::getCachedTextureCount() {
    return textureCache.size();
}

size_t TextureManager::getGpuTextureCount() {
    return std::count_if(textureCache.begin(), textureCache.end(),
        [](const auto& pair) { return pair.second.inGpu; });
}
