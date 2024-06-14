#include<glm/glm.hpp>
#include<string>
#include<vector>


using namespace std;

struct Light{
    glm::vec3 Position;
    glm::vec3 Color;
};

class LightManager{
    protected:
    LightManager(){

    }

    static LightManager* manager;
    vector<Light> lightSources;

    public:

    LightManager(LightManager &other) = delete;

    void operator = (const LightManager &) = delete;

    static LightManager * GetInstance();

    void addLight(glm::vec3 position, glm::vec3 color);

    void addLight(Light light);

    //Retorna um array de vec3 que será passado para o shader como um array e uniforms
    glm::vec3 * getLightPositions();


    glm::vec3 * getLightColors();
};

