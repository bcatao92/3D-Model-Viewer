#include<glm/glm.hpp>
#include<string>
#include<vector>
#include<glad/glad.h>

using namespace std;

struct lightVector{
    GLdouble x;
    GLdouble y;
    GLdouble z;
    lightVector(glm::vec3 vec){
        x = vec.x;
        y = vec.y;
        z = vec.z;
    }
    operator GLdouble*(){

    }
};

class LightManager{
    protected:
    LightManager(){
        lightMatrix = (GLdouble**)malloc(sizeof(lightVector*)*2);
        lightMatrix[0] = (GLdouble*)malloc(sizeof(lightVector)*128*3);
        lightMatrix[1] = (GLdouble*)malloc(sizeof(lightVector)*128*3);
    }

    static LightManager* manager;
    size_t lightNum;
    GLdouble ** lightMatrix;
    bool hasChanged;

    public:

    LightManager(LightManager &other) = delete;

    void operator = (const LightManager &) = delete;

    static LightManager * GetInstance();

    void addLight(glm::vec3 position, glm::vec3 color);

    //Retorna um array de vec3 que será passado para o shader como um array e uniforms
    GLdouble ** getLights();

    //Number of lights
    size_t getLightNum();
};

