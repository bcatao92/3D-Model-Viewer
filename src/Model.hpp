#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../external/Shader.hpp"
#include "../external/stb_image/stb_image.h"

using namespace std;

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture {
    unsigned int id;
    string type;
};

//Cada mesh representa uma primitiva do modelo
class Mesh {
    public:
        // mesh data
        vector<Vertex>       vertices;
        vector<unsigned int> indices;
        vector<Texture>      textures;

        //É necessário traduzir o objeto assimp aimesh para remover os elementos inutilizados
        //Para tal, é usado esse construtor na função "processMeshes" do modelo
        Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);

        //Permite que um modelo seja desenhado com shaders arbitrários
        void Draw(Shader &shader, size_t numberOfLights, GLdouble ** lightMatrix);
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
        //Path para carregar o modelo
        Model(char *path)
        {
            loadModel(path);
        }
        void Draw(Shader &shader);	
    private:
        // model data
        vector<Mesh> meshes;
        string directory;

        //Chamada da função de importar do assimp e das demais funções de processamento
        //e assimp retorna um objeto do tipo scene, que é composto por diversos nós, que devem
        //sem processados em meshes
        void loadModel(string path);
        
        //Função recursiva para processar os nós 
        void processNode(aiNode *node, const aiScene *scene);

        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
        
        vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
};