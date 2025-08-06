#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include <iostream>
#include <clocale>
#include <string>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Logger.cpp>

GLFWwindow* window = nullptr;

float vertices[] = {
    -0.5f, -0.5f, 0.0f, // левая вершина
     0.5f, -0.5f, 0.0f, // правая вершина
     0.0f,  0.5f, 0.0f  // верхняя вершина
};

unsigned int VBO, VAO;

const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        void main() {
            gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
        }
    )";

const char* fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;
        void main() {
            FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); // оранжевый цвет
        }
    )";

unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

unsigned int shaderProgram = glCreateProgram();

GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");

std::string readFile(const char* filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error occure opening file: " << filePath << std::endl;

        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

GLFWmonitor* getBestMonitor() {
    int count;
    GLFWmonitor** monitors = glfwGetMonitors(&count);

	GLFWmonitor* bestMonitor = nullptr;
	int bestWidth = 0, bestHeight = 0;

    for (int i = 0; i < count; i++) {
        const GLFWvidmode* mode = glfwGetVideoMode(monitors[i]);
        if (mode->width > bestWidth || (mode->width == bestWidth && mode->height > bestHeight)) {
            bestWidth = mode->width;
            bestHeight = mode->height;
            bestMonitor = monitors[i];
        }
    }

	return bestMonitor;
}

// Обработчик изменения размера окна
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Обработчик ошибок GLFW
void error_callback(int error, const char* description) {
    std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
}

int GLFWinit() {
    // Инициализация GLFW
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
}

void GLFWconfigure() {
    // Настройка GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Версия максимального OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // Версия минимального OpenGL 3.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Используем Core Profile
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE); // Разрешаем изменение размера окна
    glfwWindowHint(GLFW_SAMPLES, 4); // Антиалиасинг 4x
	glfwWindowHint(GLFW_DECORATED, GL_TRUE); // Окно с рамкой
}

GLFWwindow* getWindow() {
    GLFWmonitor* monitor = getBestMonitor();

    // Создание окна
    window = glfwCreateWindow(800, 600, "History Creator", NULL, NULL);

	// Проверка успешности создания окна
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }

	return window;
}

void moveToNeededMonitor(GLFWwindow* window) {
    GLFWmonitor* monitor = getBestMonitor();
    if (monitor) {
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        int xPos = (mode->width - 800) / 2; // Центрируем окно по горизонтали
        int yPos = (mode->height - 600) / 2; // Центрируем окно по вертикали
        glfwSetWindowPos(window, xPos, yPos);
    }
}

void printGLInfo() {
    const GLubyte* renderer = glGetString(GL_RENDERER); // Получаем имя рендерера
    const GLubyte* version = glGetString(GL_VERSION); // Получаем версию OpenGL
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL version supported: " << version << std::endl;
}

void InitializeGLAD() {
    // Инициализация GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        exit(-1);
    }
}

void LoadGL() {
    // Инициализация GLAD
    InitializeGLAD();
    // Вывод информации о OpenGL
    printGLInfo();
}

void LoadWindow() {
    // Инициализация GLFW
    GLFWinit();

    // Настройка GLFW
    GLFWconfigure();

    window = getWindow();

    moveToNeededMonitor(window);

    // Делаем окно текущим контекстом
    glfwMakeContextCurrent(window);
}

void CreateVBO() {
        // Генерация VBO
    glGenBuffers(1, &VBO);
    // Привязка VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Копирование вершин в буфер
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}
void CreateVAO() {
    // Привязка VAO
    glBindVertexArray(VAO);
    // Копирование вершин в буфер
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Установка указателей на атрибуты вершин
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void vertexShaderCompile() {
    // Компиляция вершинного шейдера
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
}

void fragmentShaderCompile() {
    // Компиляция фрагментного шейдера

    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
}

void shaderCompile() {
    vertexShaderCompile();
    fragmentShaderCompile();
    // Проверка на ошибки компиляции вершинного шейдера
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Проверка на ошибки компиляции фрагментного шейдера
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}

void createShaderProgram() {
    // Привязка шейдеров к программе
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    // Линковка программы
    glLinkProgram(shaderProgram);
    // Проверка на ошибки линковки программы
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
}

void deleteShaderProgram() {
    // Удаление шейдеров
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    // Удаление программы шейдеров
    glDeleteProgram(shaderProgram);
}

void LoadShaders() {
	// Создание VAO и VBO
    CreateVBO();
    CreateVAO();

    // Компиляция шейдеров
    shaderCompile();
    // Создание программы шейдеров
    createShaderProgram();

    deleteShaderProgram();
}

void mainRender(GLFWwindow* window, unsigned int shaderProgram, unsigned int VAO, GLuint projectionLoc) {
    // Очистка буфера цвета
    glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glClearColor(0.07f, 0.13f, 0.17f, 1.0f); // Красивый синий
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Обмен буферов и обработка событий
    glfwSwapBuffers(window);
    glfwPollEvents();
}

int main() {

	Logger::init("Main_log.log");

    setlocale(LC_ALL, "Russian");

    LoadWindow();

    LoadGL();

    LoadShaders();

    // Установка обработчиков
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Основной цикл рендеринга
	mainRender(window, shaderProgram, VAO);

    // Очистка ресурсов
    glfwTerminate();
    return 0;
}

