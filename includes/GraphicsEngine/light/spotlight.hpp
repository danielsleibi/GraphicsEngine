#pragma once
#include <glm/glm.hpp>
#include <GraphicsEngine/shader.hpp>

namespace GE
{

    class SpotLight
    {

    public:
        SpotLight(glm::vec3 position, glm::vec3 direction);
        SpotLight(glm::vec3 ambient, glm ::vec3 diffuse, glm::vec3 specular, glm::vec3 position, glm::vec3 direction, float constant, float linear, float quadratic, float cutOff, float outerCutOff);
        ~SpotLight();
        glm::vec3 getAmbient();
        void setAmbient(glm::vec3 ambient);
        glm::vec3 getDiffuse();
        void setDiffuse(glm::vec3 diffuse);
        glm::vec3 getSpecular();
        void setSpecular(glm::vec3 specular);
        glm::vec3 getPosition();
        void setPosition(glm::vec3 position);
        glm::vec3 getDirection();
        void setDirection(glm::vec3 direction);
        float getConstant();
        void setConstant(float constant);
        float getLinear();
        void setLinear(float linear);
        float getQuadratic();
        void setQuadratic(float quadratic);
        float getCutOff();
        void setCutOff(float cutOff);
        float getOuterCutOff();
        void setOuterCutOff(float outerCutOff);
        void setup(Shader &shader);

    private:
        static unsigned int MAX_LIGHTS;
        static unsigned int NR_LIGHTS;
        static unsigned int SHADER_INDEX;
        glm::vec3 ambient;
        glm ::vec3 diffuse;
        glm::vec3 specular;
        glm::vec3 position;
        glm::vec3 direction;
        float constant;
        float linear;
        float quadratic;
        float cutOff;
        float outerCutOff;
    };
}