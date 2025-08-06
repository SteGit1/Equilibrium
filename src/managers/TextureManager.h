#pragma once

#include <unordered_map>
#include <string>
#include "glad/glad.h"

class TextureManager {
private:
    static std::unordered_map<std::string, GLuint> textureCache;

public:
    static GLuint loadTexture(const std::string& path);
    static void freeTexture(const std::string& path);
    static GLuint getTexture(const std::string& path);
};