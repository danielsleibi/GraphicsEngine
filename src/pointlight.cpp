#include <GraphicsEngine/light/pointlight.hpp>
#include <iostream>

unsigned int GE::PointLight::MAX_LIGHTS = 100;
unsigned int GE::PointLight::NR_LIGHTS = 0;
unsigned int GE::PointLight::SHADER_INDEX = 0;

GE::PointLight::PointLight(glm::vec3 position)
{
    GE::PointLight::NR_LIGHTS++;
    ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    specular = glm::vec3(1.0f, 1.0f, 1.0f);
    constant = 1.0f;
    linear = 0.09f;
    quadratic = 0.032f;
    this->position = position;
}
GE::PointLight::PointLight(glm::vec3 ambient, glm ::vec3 diffuse, glm::vec3 specular, glm::vec3 position, float constant, float linear, float quadratic)
{

    GE::PointLight::NR_LIGHTS++;
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
    this->constant = constant;
    this->linear = linear;
    this->quadratic = quadratic;
    this->position = position;
}
GE::PointLight::~PointLight()
{
    GE::PointLight::NR_LIGHTS--;
    GE::PointLight::SHADER_INDEX--;
    if (GE::PointLight::SHADER_INDEX < 0)
        GE::PointLight::SHADER_INDEX = 0;
}
glm::vec3 GE::PointLight::getAmbient()
{
    return ambient;
}

void GE::PointLight::setAmbient(glm::vec3 ambient)
{
    this->ambient = ambient;
}

glm::vec3 GE::PointLight::getDiffuse()
{
    return diffuse;
}

void GE::PointLight::setDiffuse(glm::vec3 diffuse)
{
    this->diffuse = diffuse;
}

glm::vec3 GE::PointLight::getSpecular()
{
    return specular;
}

void GE::PointLight::setSpecular(glm::vec3 specular)
{
    this->specular = specular;
}

glm::vec3 GE::PointLight::getPosition()
{
    return position;
}

void GE::PointLight::setPosition(glm::vec3 position)
{
    this->position = position;
}

float GE::PointLight::getConstant()
{
    return constant;
}

void GE::PointLight::setConstant(float constant)
{
    this->constant = constant;
}

float GE::PointLight::getLinear()
{
    return linear;
}

void GE::PointLight::setLinear(float linear)
{
    this->linear = linear;
}

float GE::PointLight::getQuadratic()
{
    return quadratic;
}

void GE::PointLight::setQuadratic(float quadratic)
{
    this->quadratic = quadratic;
}

void GE::PointLight::setup(Shader &shader)
{
    shader.setInt("pointLightNr", GE::PointLight::NR_LIGHTS);
    shader.setVec("pointLights[" + std::to_string(GE::PointLight::SHADER_INDEX) + "].ambient", ambient);
    shader.setVec("pointLights[" + std::to_string(GE::PointLight::SHADER_INDEX) + "].diffuse", diffuse); // darkened
    shader.setVec("pointLights[" + std::to_string(GE::PointLight::SHADER_INDEX) + "].specular", specular);
    shader.setVec("pointLights[" + std::to_string(GE::PointLight::SHADER_INDEX) + "].position", position);
    shader.setFloat("pointLights[" + std::to_string(GE::PointLight::SHADER_INDEX) + "].constant", constant);
    shader.setFloat("pointLights[" + std::to_string(GE::PointLight::SHADER_INDEX) + "].linear", linear);
    shader.setFloat("pointLights[" + std::to_string(GE::PointLight::SHADER_INDEX) + "].quadratic", quadratic);
    GE::PointLight::SHADER_INDEX++;
    if (GE::PointLight::SHADER_INDEX >= GE::PointLight::NR_LIGHTS || GE::PointLight::SHADER_INDEX >= GE::PointLight::MAX_LIGHTS)
        GE::PointLight::SHADER_INDEX = 0;
}
