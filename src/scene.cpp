#include <GraphicsEngine/scene/scene.hpp>

GE::Scene::Scene() // dont forget to add default constructor to dir light
    : dirLight(glm::vec3(0.0f, 1.0f, 0.0f))
{
}

GE::Scene::~Scene()
{
}
GE::DirectionLight GE::Scene::getDirectionLight()
{
    return dirLight;
}
void GE::Scene::setDirectionLight(DirectionLight light)
{
    this->dirLight = light;
}
std::vector<GE::PointLight> GE::Scene::getPointLights()
{
    return pointLights;
}
void GE::Scene::setPointLights(std::vector<PointLight> lights)
{
    this->pointLights = lights;
}
std::vector<GE::SpotLight> GE::Scene::getSpotLights()
{
    return spotLights;
}
void GE::Scene::setSpotLights(std::vector<SpotLight> lights)
{
    this->spotLights = lights;
}
std::vector<GE::Model> GE::Scene::getModels()
{
    return models;
}
void GE::Scene::setModels(std::vector<Model> models)
{
    this->models = models;
}
void GE::Scene::Draw(Shader &shader)
{
    dirLight.setup(shader);
    for (int i = 0; i < pointLights.size(); i++)
    {
        pointLights[i].setup(shader);
    }
    for (int i = 0; i < spotLights.size(); i++)
    {
        spotLights[i].setup(shader);
    }
    for (int i = 0; i < models.size(); i++)
    {
        models[i].Draw(shader);
    }
}
