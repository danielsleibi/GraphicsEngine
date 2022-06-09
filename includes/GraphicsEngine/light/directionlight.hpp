#pragma once
#include <glm/glm.hpp>
#include <GraphicsEngine/shader.hpp>

namespace GE
{
    class DirectionLight
    {

    public:
        DirectionLight(glm::vec3 direction);
        DirectionLight(glm::vec3 ambient, glm ::vec3 diffuse, glm::vec3 specular, glm::vec3 position, float constant, float linear, float quadratic);
        glm::vec3 getAmbient();
        void setAmbient(glm::vec3 ambient);
        glm::vec3 getDiffuse();
        void setDiffuse(glm::vec3 diffuse);
        glm::vec3 getSpecular();
        void setSpecular(glm::vec3 specular);
        glm::vec3 getDirection();
        void setDirection(glm::vec3 direction);

        void setup(Shader &shader);

    private:
        glm::vec3 ambient;
        glm ::vec3 diffuse;
        glm::vec3 specular;
        glm::vec3 direction;
    };
}