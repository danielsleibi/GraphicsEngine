#pragma once
#include <vector>
#include <GraphicsEngine/light/directionlight.hpp>
#include <GraphicsEngine/light/pointlight.hpp>
#include <GraphicsEngine/light/spotlight.hpp>
#include <GraphicsEngine/model.hpp>

namespace GE
{
    class Scene
    {
    public:
        Scene();
        ~Scene();
        DirectionLight getDirectionLight();
        void setDirectionLight(DirectionLight light);
        std::vector<PointLight> getPointLights();
        void setPointLights(std::vector<PointLight> lights);
        std::vector<SpotLight> getSpotLights();
        void setSpotLights(std::vector<SpotLight> lights);
        std::vector<Model> getModels();
        void setModels(std::vector<Model> models);
        void Draw(Shader &shader);
        // void serialize(std::string path) implement later

    private:
        DirectionLight dirLight;
        std::vector<PointLight> pointLights;
        std::vector<SpotLight> spotLights;
        std::vector<Model> models;
    };
}