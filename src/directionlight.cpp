#include <GraphicsEngine/light/directionlight.hpp>

GE::DirectionLight::DirectionLight(glm::vec3 direction)
{
    //(-0.2f, -1.0f, -0.3f) is a good direction for default, should i set it?

    ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    specular = glm::vec3(1.0f, 1.0f, 1.0f);
    this->direction = direction;
}
GE::DirectionLight::DirectionLight(glm::vec3 ambient, glm ::vec3 diffuse, glm::vec3 specular, glm::vec3 direction, float constant, float linear, float quadratic)
{

    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
    this->direction = direction;
}

glm::vec3 GE::DirectionLight::getAmbient()
{
    return ambient;
}

void GE::DirectionLight::setAmbient(glm::vec3 ambient)
{
    this->ambient = ambient;
}

glm::vec3 GE::DirectionLight::getDiffuse()
{
    return diffuse;
}

void GE::DirectionLight::setDiffuse(glm::vec3 diffuse)
{
    this->diffuse = diffuse;
}

glm::vec3 GE::DirectionLight::getSpecular()
{
    return specular;
}

void GE::DirectionLight::setSpecular(glm::vec3 specular)
{
    this->specular = specular;
}

glm::vec3 GE::DirectionLight::getDirection()
{
    return direction;
}

void GE::DirectionLight::setDirection(glm::vec3 direction)
{
    this->direction = direction;
}

void GE::DirectionLight::setup(Shader &shader)
{
    shader.setVec("dirLight.ambient", ambient);
    shader.setVec("dirLight.diffuse", diffuse); // darkened
    shader.setVec("dirLight.specular", specular);
    shader.setVec("dirLight.direction", direction);
}
