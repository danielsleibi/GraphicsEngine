#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <glad/glad.h>
#include "shader.hpp"
namespace GE
{

    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoords;
    };

    struct Texture
    {
        unsigned int id;
        std::string type;
        std::string path; // store path of texture to compare with other textures
    };

    struct Material
    {
        glm::vec3 colorAmbient;
        glm::vec3 colorDiffuse;
        glm::vec3 colorSpecular;
        float shininess;
        std::vector<Texture> textures;
    };

    class Mesh
    {
    public:
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        Material material;

        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Material material);
        void Draw(Shader &shader);

    private:
        unsigned int VAO, VBO, EBO;
        void setupMesh();
    };
}