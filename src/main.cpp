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
    -0.5f, -0.5f, 0.0f, // ����� �������
     0.5f, -0.5f, 0.0f, // ������ �������
     0.0f,  0.5f, 0.0f  // ������� �������
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
            FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); // ��������� ����
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

// ���������� ��������� ������� ����
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// ���������� ������ GLFW
void error_callback(int error, const char* description) {
    std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
}

int GLFWinit() {
    // ������������� GLFW
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
}

void GLFWconfigure() {
    // ��������� GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // ������ ������������� OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // ������ ������������ OpenGL 3.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // ���������� Core Profile
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE); // ��������� ��������� ������� ����
    glfwWindowHint(GLFW_SAMPLES, 4); // ������������ 4x
	glfwWindowHint(GLFW_DECORATED, GL_TRUE); // ���� � ������
}

GLFWwindow* getWindow() {
    GLFWmonitor* monitor = getBestMonitor();

    // �������� ����
    window = glfwCreateWindow(800, 600, "History Creator", NULL, NULL);

	// �������� ���������� �������� ����
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
        int xPos = (mode->width - 800) / 2; // ���������� ���� �� �����������
        int yPos = (mode->height - 600) / 2; // ���������� ���� �� ���������
        glfwSetWindowPos(window, xPos, yPos);
    }
}

void printGLInfo() {
    const GLubyte* renderer = glGetString(GL_RENDERER); // �������� ��� ���������
    const GLubyte* version = glGetString(GL_VERSION); // �������� ������ OpenGL
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL version supported: " << version << std::endl;
}

void InitializeGLAD() {
    // ������������� GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        exit(-1);
    }
}

void LoadGL() {
    // ������������� GLAD
    InitializeGLAD();
    // ����� ���������� � OpenGL
    printGLInfo();
}

void LoadWindow() {
    // ������������� GLFW
    GLFWinit();

    // ��������� GLFW
    GLFWconfigure();

    window = getWindow();

    moveToNeededMonitor(window);

    // ������ ���� ������� ����������
    glfwMakeContextCurrent(window);
}

void CreateVBO() {
        // ��������� VBO
    glGenBuffers(1, &VBO);
    // �������� VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // ����������� ������ � �����
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}
void CreateVAO() {
    // �������� VAO
    glBindVertexArray(VAO);
    // ����������� ������ � �����
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // ��������� ���������� �� �������� ������
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void vertexShaderCompile() {
    // ���������� ���������� �������
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
}

void fragmentShaderCompile() {
    // ���������� ������������ �������

    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
}

void shaderCompile() {
    vertexShaderCompile();
    fragmentShaderCompile();
    // �������� �� ������ ���������� ���������� �������
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // �������� �� ������ ���������� ������������ �������
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}

void createShaderProgram() {
    // �������� �������� � ���������
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    // �������� ���������
    glLinkProgram(shaderProgram);
    // �������� �� ������ �������� ���������
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
}

void deleteShaderProgram() {
    // �������� ��������
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    // �������� ��������� ��������
    glDeleteProgram(shaderProgram);
}

void LoadShaders() {
	// �������� VAO � VBO
    CreateVBO();
    CreateVAO();

    // ���������� ��������
    shaderCompile();
    // �������� ��������� ��������
    createShaderProgram();

    deleteShaderProgram();
}

void mainRender(GLFWwindow* window, unsigned int shaderProgram, unsigned int VAO, GLuint projectionLoc) {
    // ������� ������ �����
    glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glClearColor(0.07f, 0.13f, 0.17f, 1.0f); // �������� �����
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // ����� ������� � ��������� �������
    glfwSwapBuffers(window);
    glfwPollEvents();
}

int main() {

	Logger::init("Main_log.log");

    setlocale(LC_ALL, "Russian");

    LoadWindow();

    LoadGL();

    LoadShaders();

    // ��������� ������������
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // �������� ���� ����������
	mainRender(window, shaderProgram, VAO);

    // ������� ��������
    glfwTerminate();
    return 0;
}

