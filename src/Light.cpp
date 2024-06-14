#include "Light.hpp"

LightManager* LightManager::manager = nullptr;

LightManager* LightManager::GetInstance(){
    if(manager == nullptr){
        manager = new LightManager();
    }
    return manager;
}

void LightManager::addLight(glm::vec3 position, glm::vec3 color){
    if(lightNum >= 128)
        return;
    lightVector newLightPosition(position);
    lightVector newLightColor(color);
    lightMatrix[0][lightNum] = newLightPosition;
    lightMatrix[1][lightNum] = newLightColor;
    lightNum++;
    hasChanged = true;
}

// void LightManager::addLight(Light light){
//     if(lightSources.size() >= 128)
//         return;
//     lightSources.push_back(light);
//     lightNum++;
//     hasChanged = true;
// }

lightVector ** LightManager::getLights(){
    // if(!hasChanged)
    //     return lightMatrix;
    // size_t size = lightSources.size(); 
    // GLdouble * positions = (GLdouble*)malloc(sizeof(GLdouble)*size*3);
    // GLdouble * colors = (GLdouble*)malloc(sizeof(GLdouble)*size*3);
    // GLdouble ** newMatrix = (GLdouble**)malloc(sizeof(GLdouble*)*2);
    // for(int i = 0; i < size; i += 3){
    //     positions[i] = lightSources[i].Position.x;
    //     positions[i+1] = lightSources[i].Position.y;
    //     positions[i+2] = lightSources[i].Position.z;
    //     colors[i] = lightSources[i].Color.x;
    //     colors[i+1] = lightSources[i].Color.y;
    //     colors[i+2] = lightSources[i].Color.z;
    // }
    // newMatrix[0] = positions;
    // newMatrix[1] = colors;
    // free(lightMatrix);
    // lightMatrix = newMatrix;
    // hasChanged = false;
    // return newMatrix;
    return lightMatrix;
}

size_t LightManager::getLightNum(){
    return lightNum;
}