#pragma once
#include<glm/glm.hpp>
#include<string>
#include<vector>
#include<glad/glad.h>

#define MAX_LIGHT_NUM 128

using namespace std;

class LightManager{
    public:

    LightManager(LightManager &other) = delete;

    void operator = (const LightManager &) = delete;

    static LightManager * GetInstance();

    void addLight(glm::vec3 position, glm::vec3 color);

    //Retorna um array de vec3 que será passado para o shader como um array e uniforms
    GLdouble ** getLights(){
        return lightMatrix;
    }

    //Number of lights
    size_t getLightNum(){
        return lightNum;
    }

    private:
    LightManager();

    static LightManager* manager;
    size_t lightNum;
    GLdouble ** lightMatrix;
    bool hasChanged;
};

