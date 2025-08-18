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
#include <logger.cpp>

GLFWwindow* window = nullptr;

float vertices[] = {
    -0.5f, -0.5f, 0.0f, // ëåâàÿ âåðøèíà
     0.5f, -0.5f, 0.0f, // ïðàâàÿ âåðøèíà
     0.0f,  0.5f, 0.0f  // âåðõíÿÿ âåðøèíà
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
            FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); // îðàíæåâûé öâåò
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


// Îáðàáîò÷èê èçìåíåíèÿ ðàçìåðà îêíà
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Îáðàáîò÷èê îøèáîê GLFW
void error_callback(int error, const char* description) {
	
    std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
}

int GLFWinit() {
    // Èíèöèàëèçàöèÿ GLFW
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
		logger.ERROR("Failed to initialize GLFW")
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
}

void GLFWconfigure() {
    // Íàñòðîéêà GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Âåðñèÿ ìàêñèìàëüíîãî OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // Âåðñèÿ ìèíèìàëüíîãî OpenGL 3.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Èñïîëüçóåì Core Profile
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE); // Ðàçðåøàåì èçìåíåíèå ðàçìåðà îêíà
    glfwWindowHint(GLFW_SAMPLES, 4); // Àíòèàëèàñèíã 4x
	glfwWindowHint(GLFW_DECORATED, GL_TRUE); // Îêíî ñ ðàìêîé
}

GLFWwindow* getWindow() {
    GLFWmonitor* monitor = getBestMonitor();

    // Ñîçäàíèå îêíà
    window = glfwCreateWindow(800, 600, "History Creator", NULL, NULL);

	// Ïðîâåðêà óñïåøíîñòè ñîçäàíèÿ îêíà
    if (!window) {
		logger.ERROR("Failed to create GLFW window. Object window: " << window.c_str())
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }

	return window;
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

/**
 * It move GLFW window on specifed monitor.
 *
 * @param window indicator on needed to move window
 * @param chosedMonitor indicator chosed monitor. 
 *                     if nullptr, automaticly will be selected best monitor.
 *
 * Function prove specifed monitor and if monitor don't chose it chose best monitor automaticly based on best resolution.
 */
void moveToNeededMonitor(GLFWwindow* window, GLFWmonitor* chosedMonitor) {
	if (!chosedMonitor) {
    	chosedMonitor = getBestMonitor();
	}
	else {
		changeMonitor(chosedMonitor)
	}
}

void changeMonitor(GLFWmonitor* monitor) {
    if (monitor) {
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        int xPos = (mode->width - 800) / 2; // Öåíòðèðóåì îêíî ïî ãîðèçîíòàëè
        int yPos = (mode->height - 600) / 2; // Öåíòðèðóåì îêíî ïî âåðòèêàëè
        glfwSetWindowPos(window, xPos, yPos);
    }
}

void printGLInfo() {
    const GLubyte* renderer = glGetString(GL_RENDERER); // Ïîëó÷àåì èìÿ ðåíäåðåðà
    const GLubyte* version = glGetString(GL_VERSION); // Ïîëó÷àåì âåðñèþ OpenGL
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL version supported: " << version << std::endl;
}

void InitializeGLAD() {
    // Èíèöèàëèçàöèÿ GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        exit(-1);
    }
}

void LoadGL() {
    // Èíèöèàëèçàöèÿ GLAD
    InitializeGLAD();
    // Âûâîä èíôîðìàöèè î OpenGL
    printGLInfo();
}

void LoadWindow() {
    // Èíèöèàëèçàöèÿ GLFW
    GLFWinit();

    // Íàñòðîéêà GLFW
    GLFWconfigure();

    window = getWindow();

    moveToNeededMonitor(window);

    // Äåëàåì îêíî òåêóùèì êîíòåêñòîì
    glfwMakeContextCurrent(window);
}

void CreateVBO() {
        // Ãåíåðàöèÿ VBO
    glGenBuffers(1, &VBO);
    // Ïðèâÿçêà VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Êîïèðîâàíèå âåðøèí â áóôåð
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}
void CreateVAO() {
    // Ïðèâÿçêà VAO
    glBindVertexArray(VAO);
    // Êîïèðîâàíèå âåðøèí â áóôåð
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Óñòàíîâêà óêàçàòåëåé íà àòðèáóòû âåðøèí
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void vertexShaderCompile() {
    // Êîìïèëÿöèÿ âåðøèííîãî øåéäåðà
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
}

void fragmentShaderCompile() {
    // Êîìïèëÿöèÿ ôðàãìåíòíîãî øåéäåðà

    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
}

void shaderCompile() {
    vertexShaderCompile();
    fragmentShaderCompile();
    // Ïðîâåðêà íà îøèáêè êîìïèëÿöèè âåðøèííîãî øåéäåðà
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Ïðîâåðêà íà îøèáêè êîìïèëÿöèè ôðàãìåíòíîãî øåéäåðà
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}

void createShaderProgram() {
    // Ïðèâÿçêà øåéäåðîâ ê ïðîãðàììå
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    // Ëèíêîâêà ïðîãðàììû
    glLinkProgram(shaderProgram);
    // Ïðîâåðêà íà îøèáêè ëèíêîâêè ïðîãðàììû
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
}

void deleteShaderProgram() {
    // Óäàëåíèå øåéäåðîâ
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    // Óäàëåíèå ïðîãðàììû øåéäåðîâ
    glDeleteProgram(shaderProgram);
}

void LoadShaders() {
	// Ñîçäàíèå VAO è VBO
    CreateVBO();
    CreateVAO();

    // Êîìïèëÿöèÿ øåéäåðîâ
    shaderCompile();
    // Ñîçäàíèå ïðîãðàììû øåéäåðîâ
    createShaderProgram();

    deleteShaderProgram();
}

void mainRender(GLFWwindow* window, unsigned int shaderProgram, unsigned int VAO, GLuint projectionLoc) {
    // Î÷èñòêà áóôåðà öâåòà
    glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glClearColor(0.07f, 0.13f, 0.17f, 1.0f); // Êðàñèâûé ñèíèé
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Îáìåí áóôåðîâ è îáðàáîòêà ñîáûòèé
    glfwSwapBuffers(window);
    glfwPollEvents();
}

int main() {

	Logger::init("Main_log.log");

    setlocale(LC_ALL, "Russian");

    LoadWindow();

    LoadGL();

    LoadShaders();

    // Óñòàíîâêà îáðàáîò÷èêîâ
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Îñíîâíîé öèêë ðåíäåðèíãà
	mainRender(window, shaderProgram, VAO);

    // Î÷èñòêà ðåñóðñîâ
    glfwTerminate();
    return 0;
}

