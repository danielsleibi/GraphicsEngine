

/**
 * @file test1.cpp
 * @author Daniel Sleibi (sleibi48@gmail.com)
 * @brief Tests model loading, by loading a few models
 * @version 0.1
 * @date 2022-04-24
 *
 * @copyright Copyright (c) 2022
 *
 *
 */

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstring>

#include <GraphicsEngine/shader.hpp>
#include <GraphicsEngine/camera.hpp>
#include <GraphicsEngine/model.hpp>

#define MODEL_MATRIX "model"
#define VIEW_MATRIX "view"
#define PROJECTION_MATRIX "projection"

const char *vertexLightSrcShaderPath = "../res/shaders/LightningSrc.vs";
const char *fragmentLightSrcShaderPath = "../res/shaders/LightningSrc.fs";

const char *vertexModelShaderPath = "../res/shaders/model.vs";
const char *fragmentModelShaderPath = "../res/shaders/model.fs";

GE::Shader *shader;
GE::Shader *lightCubeShader;

float width = 800.0f, height = 600.0f;
float fov = 45.0f;
float aspectRatio = width / height;

float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

GE::Camera *camera;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

unsigned int createPlaneVBO();
unsigned int createPlaneEBO();
unsigned int createCubeVBO();

int main(int argc, char **argv)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "Learning OpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    createCubeVBO();
    glBindVertexArray(0);

    GE::Shader s(vertexModelShaderPath, fragmentModelShaderPath);
    shader = &s;

    GE::Shader lcs(vertexLightSrcShaderPath, fragmentLightSrcShaderPath);
    lightCubeShader = &lcs;

    GE::Camera c;
    camera = &c;
    std::string path = "../res/models/backpack/backpack.obj";
    // path = "../res/models/crypt/12.obj";
    GE::Model backpackModel("../res/models/backpack/backpack.obj");
    GE::Model carModel("../res/models/car/car.obj");

    glEnable(GL_DEPTH_TEST);

    glm::vec3 pointLightPositions[] = {
        glm::vec3(0.7f, 0.2f, 2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f, 2.0f, -12.0f),
        glm::vec3(0.0f, 0.0f, -3.0f)};

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const float radius = 20.0f;
        glm::mat4 view;
        view = camera->getMatrix();

        aspectRatio = width / height;
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(fov), aspectRatio, 0.1f,
                                      100.0f);

        // glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

        shader->use();
        shader->setMatrix(VIEW_MATRIX, view);
        shader->setMatrix(PROJECTION_MATRIX, projection);
        shader->setVec("lightPos", lightPos);
        shader->setVec("viewPos", camera->getPosition());

        shader->setFloat("material.shininess", 32.0f);

        shader->setVec("dirLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
        shader->setVec("dirLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f)); // darkened
        shader->setVec("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        shader->setVec("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));

        shader->setVec("spotLight.position", camera->getPosition());
        shader->setVec("spotLight.direction", camera->getFront());
        shader->setVec("spotLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
        shader->setVec("spotLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
        shader->setVec("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        shader->setFloat("spotLight.constant", 1.0f);
        shader->setFloat("spotLight.linear", 0.09f);
        shader->setFloat("spotLight.quadratic", 0.032f);
        shader->setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        shader->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

        for (unsigned int i = 0; i < 4; i++)
        {
            shader->setVec("pointLights[" + std::to_string(i) + "].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
            shader->setVec("pointLights[" + std::to_string(i) + "].diffuse", glm::vec3(0.5f, 0.5f, 0.5f)); // darkened
            shader->setVec("pointLights[" + std::to_string(i) + "].specular", glm::vec3(1.0f, 1.0f, 1.0f));
            shader->setVec("pointLights[" + std::to_string(i) + "].position", pointLightPositions[i]);
            shader->setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0f);
            shader->setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09f);
            shader->setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.032f);
        }
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        shader->setMatrix(MODEL_MATRIX, model);

        // backpackModel.Draw(*shader);
        carModel.Draw(*shader);

        glBindVertexArray(lightVAO);
        // draw light cube

        lightCubeShader->use();
        lightCubeShader->setMatrix(VIEW_MATRIX, view);
        lightCubeShader->setMatrix(PROJECTION_MATRIX, projection);
        lightCubeShader->setVec("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
        lightCubeShader->setVec("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
        lightCubeShader->setVec("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        for (unsigned int i = 0; i < 4; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f));
            lightCubeShader->setMatrix(MODEL_MATRIX, model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    camera->lookTo(xpos, ypos);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera->move(GE::CameraDirection::FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera->move(GE::CameraDirection::BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera->move(GE::CameraDirection::LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera->move(GE::CameraDirection ::RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        camera->move(GE::CameraDirection::UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        camera->move(GE::CameraDirection ::DOWN, deltaTime);
    }
    float speed = 0.2f;

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        lightPos = lightPos - glm::vec3(speed * deltaTime, 0.0f, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        lightPos = lightPos - glm::vec3(-speed * deltaTime, 0.0f, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        lightPos = lightPos - glm::vec3(0.0f, 0.0f, speed * deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        lightPos = lightPos - glm::vec3(0.0f, 0.0f, -speed * deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    {
        lightPos = lightPos - glm::vec3(0.0f, speed * deltaTime, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    {
        lightPos = lightPos - glm::vec3(0.0f, -speed * deltaTime, 0.0f);
    }
}

unsigned int createCubeVBO()
{
    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f}; // 36 vertices

    unsigned int VBO;
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    return VBO;
}