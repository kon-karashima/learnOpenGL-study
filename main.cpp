#include <iostream>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void checkShaderError(GLuint shader);
void checkProgramError(GLuint program);

float first_triangle[] = {
    -0.5,   -0.5,   0.0,    1.0f,   0.0f,   0.0f,
     0.5,   -0.5,   0.0,    0.0f,   1.0f,   0.0f,
     0.0,    0.5,   0.0,    0.0f,   0.0f,   1.0f,
};

int main(int arg, char** argc) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    unsigned int VAO[2];
    glGenVertexArrays(2, VAO);
    unsigned int VBO[2];
    glGenBuffers(2, VBO);

    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(first_triangle), first_triangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    const GLchar* vertexShaderSource = 
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;"
    "layout (location = 1) in vec3 aColor;"
    "out vec3 vertexColor;"
    "void main() {"
    "    gl_Position = vec4(aPos, 1.0);"
    "    vertexColor = aColor;"
    "}";

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    checkShaderError(vertexShader);

    const GLchar* fragmentShaderSource = 
    "#version 330 core\n"
    "in vec3 vertexColor;"
    "out vec4 FragColor;"
    "void main() {"
    "    FragColor = vec4(vertexColor, 1.0f);"
    "}";

    // Orange
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    checkShaderError(fragmentShader);

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        float timeValue = glfwGetTime();
        float greenValue = sin(timeValue) / 2.0f + 0.5f;
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void checkShaderError(GLuint shader) {
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cout << "Some error occured: " << infoLog << std::endl;
    }
}

void checkProgramError(GLuint program) {
    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cout << "Some error occured: " << infoLog << std::endl;
    }
}
