#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include "glad/glad.h"
#include "Logger.cpp"

/**
 * @struct TextureData
 * @brief Структура для хранения данных о текстуре
 *
 * Содержит информацию о текстуре как в оперативной памяти (RAM),
 * так и в памяти графического процессора (GPU).
 */
struct TextureData {
    GLuint id = 0;              ///< OpenGL ID текстуры (0 если не загружена в GPU)
    std::vector<unsigned char> pixels; ///< Пиксельные данные текстуры в RAM
    int width = 0;              ///< Ширина текстуры в пикселях
    int height = 0;             ///< Высота текстуры в пикселях
    int channels = 0;           ///< Количество цветовых каналов (3 = RGB, 4 = RGBA)
    bool inGpu = false;         ///< Флаг, указывающий загружена ли текстура в GPU
};

/**
 * @class TextureManager
 * @brief Менеджер для управления текстурами
 *
 * Реализует паттерн Singleton для централизованного управления ресурсами текстур.
 * Обеспечивает кэширование, загрузку и выгрузку текстур из памяти.
 */
class TextureManager {
private:
    static std::unordered_map<std::string, TextureData> textureCache; ///< Кэш текстур

    /**
     * @brief Загружает текстуру в память GPU
     * @param texData Ссылка на объект TextureData с данными текстуры
     * @return OpenGL ID созданной текстуры
     *
     * Если текстура уже имеет ID (id != 0), метод обновит её данные в GPU.
     * В противном случае создаст новую текстуру.
     */
    static GLuint uploadToGpu(TextureData& textureData);

public:
    /**
     * @brief Загружает текстуру из файла
     * @param path Путь к файлу текстуры
     * @return OpenGL ID загруженной текстуры (0 при ошибке)
     *
     * - Если текстура уже в кэше, возвращает её ID
     * - Если данные есть в RAM, но не в GPU - загружает в GPU
     * - При первом обращении загружает с диска в RAM и GPU
     */
    static GLuint loadTexture(const std::string& path);

    /**
     * @brief Освобождает ресурсы текстуры
     * @param path Путь к файлу текстуры
     * @param force Принудительное удаление даже если есть ссылки
     *
     * - Удаляет текстуру из GPU (если inGpu = true)
     * - При force = true полностью удаляет запись из кэша
     * - При force = false только помечает как не в GPU
     */
    static void freeTexture(const std::string& path, bool force = false);

    /**
     * @brief Получает текстуру (псевдоним для loadTexture)
     * @param path Путь к файлу текстуры
     * @return OpenGL ID текстуры
     */
    static GLuint getTexture(const std::string& path);

    // Управление памятью GPU

    /**
     * @brief Выгружает текстуру из памяти GPU
     * @param path Путь к файлу текстуры
     *
     * Освобождает видеопамять, но сохраняет данные в RAM.
     * Полезно для управления ресурсами на слабых GPU.
     */
    static void unloadFromGpu(const std::string& path);

    /**
     * @brief Перезагружает текстуру в память GPU
     * @param path Путь к файлу текстуры
     *
     * Использует данные, сохранённые в RAM для повторной загрузки в GPU.
     * Не требует доступа к исходному файлу на диске.
     */
    static void reloadToGpu(const std::string& path);

    // Статистика

    /**
     * @brief Возвращает количество кэшированных текстур
     * @return Количество текстур в кэше (в RAM и/или GPU)
     */
    static size_t getCachedTextureCount();

    /**
     * @brief Возвращает количество текстур в памяти GPU
     * @return Количество активных текстур в видеопамяти
     */
    static size_t getGpuTextureCount();
};