#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include "glad/glad.h"
#include "Logger.cpp"

/**
 * @struct TextureData
 * @brief ��������� ��� �������� ������ � ��������
 *
 * �������� ���������� � �������� ��� � ����������� ������ (RAM),
 * ��� � � ������ ������������ ���������� (GPU).
 */
struct TextureData {
    GLuint id = 0;              ///< OpenGL ID �������� (0 ���� �� ��������� � GPU)
    std::vector<unsigned char> pixels; ///< ���������� ������ �������� � RAM
    int width = 0;              ///< ������ �������� � ��������
    int height = 0;             ///< ������ �������� � ��������
    int channels = 0;           ///< ���������� �������� ������� (3 = RGB, 4 = RGBA)
    bool inGpu = false;         ///< ����, ����������� ��������� �� �������� � GPU
};

/**
 * @class TextureManager
 * @brief �������� ��� ���������� ����������
 *
 * ��������� ������� Singleton ��� ����������������� ���������� ��������� �������.
 * ������������ �����������, �������� � �������� ������� �� ������.
 */
class TextureManager {
private:
    static std::unordered_map<std::string, TextureData> textureCache; ///< ��� �������

    /**
     * @brief ��������� �������� � ������ GPU
     * @param texData ������ �� ������ TextureData � ������� ��������
     * @return OpenGL ID ��������� ��������
     *
     * ���� �������� ��� ����� ID (id != 0), ����� ������� � ������ � GPU.
     * � ��������� ������ ������� ����� ��������.
     */
    static GLuint uploadToGpu(TextureData& textureData);

public:
    /**
     * @brief ��������� �������� �� �����
     * @param path ���� � ����� ��������
     * @return OpenGL ID ����������� �������� (0 ��� ������)
     *
     * - ���� �������� ��� � ����, ���������� � ID
     * - ���� ������ ���� � RAM, �� �� � GPU - ��������� � GPU
     * - ��� ������ ��������� ��������� � ����� � RAM � GPU
     */
    static GLuint loadTexture(const std::string& path);

    /**
     * @brief ����������� ������� ��������
     * @param path ���� � ����� ��������
     * @param force �������������� �������� ���� ���� ���� ������
     *
     * - ������� �������� �� GPU (���� inGpu = true)
     * - ��� force = true ��������� ������� ������ �� ����
     * - ��� force = false ������ �������� ��� �� � GPU
     */
    static void freeTexture(const std::string& path, bool force = false);

    /**
     * @brief �������� �������� (��������� ��� loadTexture)
     * @param path ���� � ����� ��������
     * @return OpenGL ID ��������
     */
    static GLuint getTexture(const std::string& path);

    // ���������� ������� GPU

    /**
     * @brief ��������� �������� �� ������ GPU
     * @param path ���� � ����� ��������
     *
     * ����������� �����������, �� ��������� ������ � RAM.
     * ������� ��� ���������� ��������� �� ������ GPU.
     */
    static void unloadFromGpu(const std::string& path);

    /**
     * @brief ������������� �������� � ������ GPU
     * @param path ���� � ����� ��������
     *
     * ���������� ������, ���������� � RAM ��� ��������� �������� � GPU.
     * �� ������� ������� � ��������� ����� �� �����.
     */
    static void reloadToGpu(const std::string& path);

    // ����������

    /**
     * @brief ���������� ���������� ������������ �������
     * @return ���������� ������� � ���� (� RAM �/��� GPU)
     */
    static size_t getCachedTextureCount();

    /**
     * @brief ���������� ���������� ������� � ������ GPU
     * @return ���������� �������� ������� � �����������
     */
    static size_t getGpuTextureCount();
};