#pragma once
#include <glm/glm.hpp>
#include <GraphicsEngine/shader.hpp>

namespace GE
{

    class PointLight
    {

    public:
        PointLight(glm::vec3 position);
        PointLight(glm::vec3 ambient, glm ::vec3 diffuse, glm::vec3 specular, glm::vec3 position, float constant, float linear, float quadratic);
        ~PointLight();
        glm::vec3 getAmbient();
        void setAmbient(glm::vec3 ambient);
        glm::vec3 getDiffuse();
        void setDiffuse(glm::vec3 diffuse);
        glm::vec3 getSpecular();
        void setSpecular(glm::vec3 specular);
        glm::vec3 getPosition();
        void setPosition(glm::vec3 position);
        float getConstant();
        void setConstant(float constant);
        float getLinear();
        void setLinear(float linear);
        float getQuadratic();
        void setQuadratic(float quadratic);
        void setup(Shader &shader);

    private:
        static unsigned int MAX_LIGHTS;
        static unsigned int NR_LIGHTS;
        static unsigned int SHADER_INDEX;
        glm::vec3 ambient;
        glm ::vec3 diffuse;
        glm::vec3 specular;
        glm::vec3 position;
        float constant;
        float linear;
        float quadratic;
    };
}