

/**
 * @file chess.cpp
 * @author Daniel Sleibi (sleibi48@gmail.com)
 * @brief Chess game created for implemeneting and testing scene system
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
#include <GraphicsEngine/light/directionlight.hpp>
#include <GraphicsEngine/light/pointlight.hpp>
#include <GraphicsEngine/light/spotlight.hpp>
#include <GraphicsEngine/scene/scene.hpp>

#define MODEL_MATRIX "model"
#define VIEW_MATRIX "view"
#define PROJECTION_MATRIX "projection"

const char *vertexLightSrcShaderPath = "../res/shaders/LightningSrc.vs";
const char *fragmentLightSrcShaderPath = "../res/shaders/LightningSrc.fs";

const char *vertexModelShaderPath = "../res/shaders/model.vs";
const char *fragmentModelShaderPath = "../res/shaders/model.fs";

const char *fragmentCustomColorShaderPath = "../res/shaders/CustomColor.fs";

GE::Shader *shader;
GE::Shader *lightCubeShader;

float width = 800.0f, height = 600.0f;
float fov = 45.0f;
float aspectRatio = width / height;

float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

GE::Camera *camera;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

bool enableFlashlight = true;
bool lastFState = false;

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

    GLFWwindow *window = glfwCreateWindow(800, 600, "demo_ChessGame", NULL, NULL);
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

    // prepare dir light
    GE::DirectionLight dirLight(glm::vec3(-0.2f, -1.0f, -0.3f));
    dirLight.setAmbient(glm::vec3(0.0f));
    dirLight.setDiffuse(glm::vec3(1.0f));
    dirLight.setSpecular(glm::vec3(1.0f));
    dirLight.setDirection(glm::vec3(0.0f, -1.0f, 0.0f));
    // prepare point lights
    std::vector<GE::PointLight> pointLights =
        {
            GE::PointLight(glm::vec3(3.0f, 2.0f, 3.0f)),
            // GE::PointLight(glm::vec3(2.3f, -3.3f, -4.0f)),
            // GE::PointLight(glm::vec3(-4.0f, 2.0f, -12.0f)),
            GE::PointLight(glm::vec3(-3.0f, 2.0f, -3.0f))};

    // prepare spot lights
    std::vector<GE::SpotLight> spotLights =
        {
            GE::SpotLight(glm::vec3(0.0f), glm::vec3(0.0f))};

    // prepare models
    GE::Model soldierPieceModel("../res/models/demo/ChessGame/SoldierPiece/SoldierPiece.obj");

    std::vector<GE::Model>
        models = {
            GE::Model("../res/models/demo/ChessGame/Chessboard/Chessboard.obj")};

    GE::Model sphereModel("../res/models/sphere/sphere.obj");

    // prepare scene
    GE::Scene scene;
    scene.setDirectionLight(dirLight);
    scene.setPointLights(pointLights);
    scene.setSpotLights(spotLights);

    GE::SpotLight *flashLight = &scene.getSpotLights()[0];

    // prepare grid
    std::vector<std::vector<unsigned int>> grid = {
        {1, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
    };

    for (unsigned int i = 0; i < grid.size(); i++)
    {
        for (unsigned int j = 0; j < grid.size(); j++)
        {
            unsigned int piece = grid[i][j];
            if (piece == 1)
            {
                GE::Model soldierPieceModel("../res/models/demo/ChessGame/SoldierPiece/SoldierPiece.obj");

                glm::mat4 spMat = soldierPieceModel.getMatrix();
                spMat = glm::translate(spMat, glm::vec3(0.875f - (i / 8), 0.125f, 0.875f - (j / 8)));
                spMat = glm::scale(spMat, glm::vec3(0.05f));
                soldierPieceModel.setMatrix(spMat);

                models.push_back(soldierPieceModel);
            }
        }
    }

    for (unsigned int i = 0; i < models.size(); i++)
    {
        std::string s = models[i].operator std::string();
        std::cout << s << std::endl;
    }

    scene.setModels(models);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
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

        shader->use();
        shader->setMatrix(VIEW_MATRIX, view);
        shader->setMatrix(PROJECTION_MATRIX, projection);
        shader->setVec("viewPos", camera->getPosition());

        flashLight->setPosition(camera->getPosition());
        flashLight->setDirection(camera->getFront());

        scene.Draw(*shader);

        // draw light cube
        lightCubeShader->use();
        lightCubeShader->setMatrix(VIEW_MATRIX, view);
        lightCubeShader->setMatrix(PROJECTION_MATRIX, projection);
        for (unsigned int i = 0; i < pointLights.size(); i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, pointLights[i].getPosition());
            model = glm::scale(model, glm::vec3(0.2f));

            sphereModel.setMatrix(model);
            lightCubeShader->setVec("light.ambient", pointLights[i].getAmbient());
            lightCubeShader->setVec("light.diffuse", pointLights[i].getDiffuse());
            lightCubeShader->setVec("light.specular", pointLights[i].getSpecular());

            sphereModel.Draw(*lightCubeShader);
        }

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
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        if (!lastFState)
        {
            enableFlashlight = !enableFlashlight;
            lastFState = true;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE)
    {
        if (lastFState)
            lastFState = false;
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