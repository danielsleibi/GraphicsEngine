#pragma once
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace GE
{ /**
   * @brief Represents a GLSL shader, abstracting and dealing with shader loading
   *
   */
    class Shader
    {

    public:
        // constructor reads and builds the shader
        Shader(const char *vertexPath, const char *fragmentPath);

        // use/activate the shader program
        void use();

        // utitlity uniform functions
        void setBool(const std::string &name, bool value) const;
        void setInt(const std::string &name, int value) const;
        void setFloat(const std::string &name, float value) const;
        void setVec(const std::string &name, glm::vec3 value);
        void setMatrix(const std::string &name, glm::mat4 value);

        // getter for program ID
        unsigned int getID();

    private:
        unsigned int ID;
    };
}