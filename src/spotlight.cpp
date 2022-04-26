#include <GraphicsEngine/light/spotlight.hpp>
#include <iostream>
unsigned int GE::SpotLight::MAX_LIGHTS = 100;
unsigned int GE::SpotLight::NR_LIGHTS = 0;
unsigned int GE::SpotLight::SHADER_INDEX = 0;

GE::SpotLight::SpotLight(glm::vec3 position, glm::vec3 direction)
{
    GE::SpotLight::NR_LIGHTS++;
    ambient = glm::vec3(0.0f, 0.0f, 0.0f);
    diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    specular = glm::vec3(1.0f, 1.0f, 1.0f);
    constant = 1.0f;
    linear = 0.09f;
    quadratic = 0.032f;
    this->cutOff = glm::cos(glm::radians(12.5f));
    this->outerCutOff = glm::cos(glm::radians(15.0f));
    this->position = position;
}
GE::SpotLight::SpotLight(glm::vec3 ambient, glm ::vec3 diffuse, glm::vec3 specular, glm::vec3 position, glm::vec3 direction, float constant, float linear, float quadratic, float cutOff, float outerCutOff)
{

    GE::SpotLight::NR_LIGHTS++;
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
    this->constant = constant;
    this->linear = linear;
    this->quadratic = quadratic;
    this->cutOff = cutOff;
    this->outerCutOff = outerCutOff;
    this->position = position;
    this->direction = direction;
}
GE::SpotLight::~SpotLight()
{
    GE::SpotLight::NR_LIGHTS--;
    GE::SpotLight::SHADER_INDEX--;
    if (GE::SpotLight::SHADER_INDEX < 0)
        GE::SpotLight::SHADER_INDEX = 0;
}
glm::vec3 GE::SpotLight::getAmbient()
{
    return ambient;
}

void GE::SpotLight::setAmbient(glm::vec3 ambient)
{
    this->ambient = ambient;
}

glm::vec3 GE::SpotLight::getDiffuse()
{
    return diffuse;
}

void GE::SpotLight::setDiffuse(glm::vec3 diffuse)
{
    this->diffuse = diffuse;
}

glm::vec3 GE::SpotLight::getSpecular()
{
    return specular;
}

void GE::SpotLight::setSpecular(glm::vec3 specular)
{
    this->specular = specular;
}

glm::vec3 GE::SpotLight::getPosition()
{
    return position;
}

void GE::SpotLight::setPosition(glm::vec3 position)
{
    this->position = position;
}

glm::vec3 GE::SpotLight::getDirection()
{
    return direction;
}

void GE::SpotLight::setDirection(glm::vec3 direction)
{
    this->direction = direction;
}

float GE::SpotLight::getConstant()
{
    return constant;
}

void GE::SpotLight::setConstant(float constant)
{
    this->constant = constant;
}

float GE::SpotLight::getLinear()
{
    return linear;
}

void GE::SpotLight::setLinear(float linear)
{
    this->linear = linear;
}

float GE::SpotLight::getQuadratic()
{
    return quadratic;
}

void GE::SpotLight::setQuadratic(float quadratic)
{
    this->quadratic = quadratic;
}

float GE::SpotLight::getCutOff()
{
    return cutOff;
}

void GE::SpotLight::setCutOff(float cutOff)
{
    this->cutOff = cutOff;
}

float GE::SpotLight::getOuterCutOff()
{
    return cutOff;
}

void GE::SpotLight::setOuterCutOff(float outerCutOff)
{
    this->outerCutOff = outerCutOff;
}

void GE::SpotLight::setup(Shader &shader)
{

    shader.setInt("spotLightNr", GE::SpotLight::NR_LIGHTS);
    shader.setVec("spotLights[" + std::to_string(GE::SpotLight::SHADER_INDEX) + "].ambient", ambient);
    shader.setVec("spotLights[" + std::to_string(GE::SpotLight::SHADER_INDEX) + "].diffuse", diffuse); // darkened
    shader.setVec("spotLights[" + std::to_string(GE::SpotLight::SHADER_INDEX) + "].specular", specular);
    shader.setVec("spotLights[" + std::to_string(GE::SpotLight::SHADER_INDEX) + "].position", position);
    shader.setVec("spotLights[" + std::to_string(GE::SpotLight::SHADER_INDEX) + "].direction", direction);
    shader.setFloat("spotLights[" + std::to_string(GE::SpotLight::SHADER_INDEX) + "].constant", constant);
    shader.setFloat("spotLights[" + std::to_string(GE::SpotLight::SHADER_INDEX) + "].linear", linear);
    shader.setFloat("spotLights[" + std::to_string(GE::SpotLight::SHADER_INDEX) + "].quadratic", quadratic);
    shader.setFloat("spotLights[" + std::to_string(GE::SpotLight::SHADER_INDEX) + "].cutOff", cutOff);
    shader.setFloat("spotLights[" + std::to_string(GE::SpotLight::SHADER_INDEX) + "].outerCutOff", outerCutOff);
    GE::SpotLight::SHADER_INDEX++;
    if (GE::SpotLight::SHADER_INDEX >= GE::SpotLight::NR_LIGHTS || GE::SpotLight::SHADER_INDEX >= GE::SpotLight::MAX_LIGHTS)
        GE::SpotLight::SHADER_INDEX = 0;
}
