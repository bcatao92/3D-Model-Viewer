#include "Light.hpp"

LightManager* LightManager::manager = nullptr;

LightManager* LightManager::GetInstance(){
    if(manager == nullptr){
        manager = new LightManager();
    }
    return manager;
}

void LightManager::addLight(glm::vec3 position, glm::vec3 color){
    if(lightNum >= MAX_LIGHT_NUM)
        return;
    lightMatrix[0][lightNum*3] = position.x;
    lightMatrix[0][lightNum*3 + 1] = position.y;
    lightMatrix[0][lightNum*3 + 2] = position.z;
    lightMatrix[1][lightNum*3] = color.x;
    lightMatrix[1][lightNum*3 + 1] = color.x;
    lightMatrix[1][lightNum*3 + 2] = color.x;
    lightNum++;
    hasChanged = true;
}

LightManager::LightManager(){
    lightMatrix = (GLdouble**)malloc(sizeof(GLdouble*)*2);
    lightMatrix[0] = (GLdouble*)malloc(sizeof(GLdouble)*MAX_LIGHT_NUM*3);
    lightMatrix[1] = (GLdouble*)malloc(sizeof(GLdouble)*MAX_LIGHT_NUM*3);
}