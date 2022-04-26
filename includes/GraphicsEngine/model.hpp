#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <stb/stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>

#include "mesh.hpp"
#include "shader.hpp"

namespace GE
{
    class Model
    {
    public:
        Model(std::string path);

        void Draw(Shader &shader);

    private:
        std::vector<Mesh> meshes;
        std::vector<Texture> textures_loaded;
        std::string directory;

        void loadModel(std::string path);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
        std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
        std::vector<glm::vec3> loadMaterialColors(aiMaterial *mat);
    };
}