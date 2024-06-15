#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include "../external/Shader.hpp"
#include "../src/Model.hpp"
#include "../src/Camera.hpp"
#include "../src/Mouse.hpp"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);  


int main(int argc, char **argv)
{

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "ModelViewer", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  

    glViewport(0, 0, 800, 600);

    glEnable(GL_DEPTH_TEST);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    stbi_set_flip_vertically_on_load(true);

    Mouse mouse(window);

    std::cout << "Carregando modelo" << std::endl;

    Model model("model/backpack.obj");

    std::cout << "Modelo Carregado" << std::endl;

    std::string vertexShaderPath = "Shaders/ModelVShader.vs";
    std::string fragmentShaderPath = "Shaders/ModelFShader.fs";
    Shader ModelShader(vertexShaderPath.c_str(), fragmentShaderPath.c_str());

    Camera cameraTeste(mouse);

    double distance = 10.0f;

    while(!glfwWindowShouldClose(window))
    {
        cameraTeste.Update();
        double time = glfwGetTime();

        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ModelShader.use();

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = cameraTeste.getViewMat();
        ModelShader.setMat4("projection", projection);
        ModelShader.setMat4("view", view);

        // render the loaded model
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        ModelShader.setMat4("model", modelMatrix);

        model.Draw(ModelShader);

        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    glfwTerminate();
    return 0;

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}  
