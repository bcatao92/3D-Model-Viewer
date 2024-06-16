#include "Light.hpp"
#include <iostream>

LightManager* LightManager::manager = nullptr;

using namespace std;

LightManager* LightManager::GetInstance(){
    if(manager == nullptr){
        manager = new LightManager();
    }
    return manager;
}

void LightManager::setAmbientLight(glm::vec3 color){
    ambientLight = color;
}

void LightManager::addLight(glm::vec3 position, glm::vec3 color){
    if(lightNum >= MAX_LIGHT_NUM)
        return;
    lightMatrix[0][lightNum*3] = position.x;
    lightMatrix[0][lightNum*3 + 1] = position.y;
    lightMatrix[0][lightNum*3 + 2] = position.z;
    lightMatrix[1][lightNum*3] = color.x;
    lightMatrix[1][lightNum*3 + 1] = color.y;
    lightMatrix[1][lightNum*3 + 2] = color.z;
    lightNum++;
}

LightManager::LightManager(){
    lightMatrix = (GLfloat**)malloc(sizeof(GLfloat*)*2);
    lightMatrix[0] = (GLfloat*)malloc(sizeof(GLfloat)*MAX_LIGHT_NUM*3);
    lightMatrix[1] = (GLfloat*)malloc(sizeof(GLdouble)*MAX_LIGHT_NUM*3);
}