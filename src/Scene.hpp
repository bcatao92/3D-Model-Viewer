#pragma once
#include "Mouse.hpp"
#include "Model.hpp"
#include "Camera.hpp"
#include "Light.hpp"
#include "utils.hpp"
#include "Skybox.hpp"
#include <glm/glm.hpp>
#include <vector>
#include <string>


class Scene{
    public:

    Scene(const char * title, int screenWidth, int screenHeight);

    void Update();

    void Draw();

    void Start();

    void addModel(Model model){
        models.push_back(model);
    }

    void addLight(glm::vec3 position, glm::vec3 color){
        lightManager.addLight(position, color);
    }

    void setAmbientLight(glm::vec3 color){
        lightManager.setAmbientLight(color);
    }

    //void setMouseCallback(void * (GLFWwindow * window, int button, int action, int mods));

    void setSensitivity(double sensitivity){
        mouse->sensitivity = sensitivity;
    }

    //Espera receber o nome de uma pasta contendo um cubemap com nomes
    //right.jpg, left.jpg, top.jpg, etc
    void addBackground(std::string cubeMapFolder);


    private:
    //Usado para o cálculo do delta t
    double lastTime;

    Mouse * mouse;

    //Câmera
    Camera * camera;

    //Luzes
    LightManager lightManager;

    //Modelos
    std::vector<Model> models;

    //Skybox
    Skybox * skybox = nullptr;

    //Atributos da janela
    GLFWwindow * window;
    int screenWidth, screenHeight;

    //Evitar que, para caso o usuário queira criar mais de uma cena, não sejam
    //chamadas as funções de carregar glad e glfw
    static bool gladLoaded;
    static bool glfwLoaded;

    //Evitar que seja chamada a função de gerar matriz de projeção a cada frame
    glm::mat4 projection;
};