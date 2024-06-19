#include "Shader.hpp"
#include <string>

class Skybox{
    public:
    Shader * shader;

    Skybox(std::string folder){
        ID = loadCubeMap(folder);
        std::string vertexShaderPath = "Shaders/CubeMapVShader.vs";
        std::string fragmentShaderPath = "Shaders/CubeMapFShader.fs";
        shader = new Shader(vertexShaderPath.c_str(), fragmentShaderPath.c_str());
        setupVAO();
    }

    void Draw();

    private:
    GLuint ID, VAO, VBO;

    GLuint loadCubeMap(std::string folder);

    void setupVAO();
};