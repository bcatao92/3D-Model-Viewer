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
        lightMatrix = (lightVector**)malloc(sizeof(lightVector*)*2);
        lightMatrix[0] = (lightVector*)malloc(sizeof(lightVector)*128);
        lightMatrix[1] = (lightVector*)malloc(sizeof(lightVector)*128);
    }

    static LightManager* manager;
    size_t lightNum;
    lightVector ** lightMatrix;
    bool hasChanged;

    public:

    LightManager(LightManager &other) = delete;

    void operator = (const LightManager &) = delete;

    static LightManager * GetInstance();

    void addLight(glm::vec3 position, glm::vec3 color);

    //void addLight(Light light);

    //Retorna um array de vec3 que será passado para o shader como um array e uniforms
    lightVector ** getLights();

    //Number of lights
    size_t getLightNum();
};

