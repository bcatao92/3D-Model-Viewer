#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include <Shader.hpp>
#include "Model.hpp"
#include "Camera.hpp"
#include "Mouse.hpp"
#include "Light.hpp"
#include "utils.hpp"
#include "Scene.hpp"

using namespace std;

double mouse_sensitivity = 5000.;

bool is_number(const std::string& s)
{
    bool foundDot = false;
    std::string::const_iterator it = s.begin();
    while (it != s.end() && (std::isdigit(*it) || (*it == '.' && foundDot==false))){ 
        if(*it == '.') foundDot = true;
        ++it;
    }
    return !s.empty() && it == s.end();
}

bool processInput(int argc, char **argv){
    if(argc < 2){
        cout << "Not enough arguments" << endl;
    }
    else if(argc == 2)
        return true;
    else if(argc == 3){
        string mouseSensitivity(argv[2]);
        if(!is_number(mouseSensitivity)){
            cout << "Mouse sensitivity must be number" << endl;}
        else{
            mouse_sensitivity = atof(argv[2]);
            return true;
        }
    }
    else
        cout << "too many arguments" << endl;
    cout << "USAGE: Sintese <path-to-model> (optional)<mouse-sensitivity>" << endl;
    return false;
}

int main(int argc, char **argv)
{

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    if(!processInput(argc, argv)) return -1;

    Scene modelViewer("3D model viewer", 800, 600);
    modelViewer.setSensitivity(mouse_sensitivity);

    string vertexShaderPath = "Shaders/ModelVShader.vs";
    string fragmentShaderPath = "Shaders/ModelFShader.fs";
    Shader * ModelShader = new Shader(vertexShaderPath.c_str(), fragmentShaderPath.c_str());

    cout << "Carregando modelo" << endl;

    string path = getPath(argv[1]);
    Model model(path.c_str(), ModelShader);
    modelViewer.addModel(model);

    cout << "Modelo Carregado" << endl;

    double distance = 10.0f;

    LightManager lightManager;

    modelViewer.addLight(glm::vec3(5.f,5.f,5.f), glm::vec3(0.f,0.f,1.f));
    modelViewer.addLight(glm::vec3(-5.f,5.f,-5.f), glm::vec3(1.f,0.f,0.f));
    modelViewer.addLight(glm::vec3(0.f,5.f,5.f), glm::vec3(1.f,1.f,1.f));
    modelViewer.setAmbientLight(glm::vec3(1.0f,1.0f,1.0f));

    modelViewer.addBackground("assets/forest-skyboxes/MountainPath");

    modelViewer.Start();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}  