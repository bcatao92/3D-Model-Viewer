#pragma once
#include <glad/include/glad/glad.h>
#include <glm/glm/glm.hpp>
#include <string>
#include <vector>

#define MAX_LIGHT_NUM 128

class LightManager{
    public:

    LightManager();

    void addLight(glm::vec3 position, glm::vec3 color);

    void setAmbientLight(glm::vec3 color);

    //Retorna um array de vec3 que será passado para o shader como um array e uniforms
    GLfloat ** getLights(){
        return lightMatrix;
    }

    //Number of lights
    size_t getLightNum(){
        return lightNum;
    }

    //Ambient light
    glm::vec3 getAmbientLight(){
        return ambientLight;
    }

    private:

    glm::vec3 ambientLight = glm::vec3(0.,0.,0.);
    size_t lightNum = 0;
    GLfloat ** lightMatrix;
};

