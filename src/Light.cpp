#include "Light.hpp"

LightManager* LightManager::manager = nullptr;

LightManager* LightManager::GetInstance(){
    if(manager == nullptr){
        manager = new LightManager();
    }
    return manager;
}

void LightManager::addLight(glm::vec3 position, glm::vec3 color){
    Light light;
    light.Color = color;
    light.Position = position;
    lightSources.push_back(light);
}

void LightManager::addLight(Light light){
    lightSources.push_back(light);
}

glm::vec3 * LightManager::getLightColors(){
    size_t size = lightSources.size();
    glm::vec3 * colors = (glm::vec3*)malloc(sizeof(glm::vec3)*size);
    for(int i = 0; i < size; i++){
        colors[i] = lightSources[i].Color;
    }
    return colors;
}

glm::vec3 * LightManager::getLightPositions(){
    size_t size = lightSources.size();
    glm::vec3 * positions = (glm::vec3*)malloc(sizeof(glm::vec3)*size);
    for(int i = 0; i < size; i++){
        positions[i] = lightSources[i].Position;
    }
    return positions;
}