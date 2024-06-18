#pragma once
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/glm.hpp>
#include <assimp/include/assimp/Importer.hpp>
#include <assimp/include/assimp/scene.h>
#include <assimp/include/assimp/postprocess.h>
#include <Shader.hpp>
#include <stb_image/stb_image.h>
#include <string>
#include <vector>
#include <map>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string name;
};

//Cada mesh representa uma primitiva do modelo
class Mesh {
    public:
        // mesh data
        std::vector<Vertex>       vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture>      textures;

        //É necessário traduzir o objeto assimp aimesh para remover os elementos inutilizados
        //Para tal, é usado esse construtor na função "processMeshes" do modelo
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures){
            this->vertices = vertices;
            this->indices = indices;
            this->textures = textures;

            // now that we have all the required data, set the vertex buffers and its attribute pointers.
            setupMesh();
        }

        //Permite que um modelo seja desenhado com shaders arbitrários
        void Draw(Shader *shader);


    private:

        //Objetos OpenGL
        unsigned int VAO, VBO, EBO;

        //Cria os objetos OpenGL e transfere a informação para a GPU
        void setupMesh();
};

//Modelo contendo vários meshes
class Model 
{
    public:
        glm::vec3 position = glm::vec3(0.,0.,0.);
        Shader * shader;

        //Path para carregar o modelo
        Model(const char *path, Shader *shader) : shader(shader)
        {
            loadModel(path);
        }

        void Draw();

        //Apenas escala 
        void Scale(double scaleFactor){
            scale *= scaleFactor;
        }

        glm::vec3 getScale(){
            return glm::vec3(scale,scale,scale);
        }

    private:
        // model data
        std::map<std::string, Texture> loaded_textures;
        std::vector<Mesh> meshes;

        //Atributos do modelo
        double scale = 1.;

        //Caminho para o modelo, será usado para encontrar as texturas posteriormente
        std::string directory;

        //Chamada da função de importar do assimp e das demais funções de processamento
        //e assimp retorna um objeto do tipo scene, que é composto por diversos nós, que devem
        //sem processados em meshes
        void loadModel(std::string path);
        
        //Função recursiva para processar os nós 
        void processNode(aiNode *node, const aiScene *scene);

        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
        
        std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};