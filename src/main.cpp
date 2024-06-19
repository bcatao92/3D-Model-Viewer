#include <glad/include/glad/glad.h>
#include <glm/glm/glm.hpp>
#include <glfw/include/GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <Shader.hpp>
#include "Model.hpp"
#include "Camera.hpp"
#include "Mouse.hpp"
#include "Light.hpp"
#include "utils.hpp"
#include "Scene.hpp"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);  


int main(int argc, char **argv)
{

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    Scene modelViewer("3D model viewer", 800, 600);

    std::string vertexShaderPath = "Shaders/ModelVShader.vs";
    std::string fragmentShaderPath = "Shaders/ModelFShader.fs";
    Shader * ModelShader = new Shader(vertexShaderPath.c_str(), fragmentShaderPath.c_str());

    std::cout << "Carregando modelo" << std::endl;

    std::string path = getPath("model/backpack.obj");
    Model model(path.c_str(), ModelShader);
    modelViewer.addModel(model);

    std::cout << "Modelo Carregado" << std::endl;

    double distance = 10.0f;

    LightManager lightManager;

    modelViewer.addLight(glm::vec3(5.f,5.f,5.f), glm::vec3(0.f,0.f,1.f));
    modelViewer.addLight(glm::vec3(-5.f,5.f,-5.f), glm::vec3(1.f,0.f,0.f));
    modelViewer.addLight(glm::vec3(0.f,5.f,5.f), glm::vec3(1.f,1.f,1.f));
    modelViewer.setAmbientLight(glm::vec3(1.0f,1.0f,1.0f));

    modelViewer.addBackground("forest-skyboxes/MountainPath");

    modelViewer.Start();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}  
