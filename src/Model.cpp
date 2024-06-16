#include "Model.hpp"
#include "Light.hpp"
#include <filesystem>

GLuint TextureFromFile(const char* name){
    std::cout << "Carregando textura: " << name << std::endl;
    GLuint textureID;

    //Essa função assume que a textura esteja no diretório do binário
    std::filesystem::path path = std::filesystem::current_path();
    std::string parentPath(path.parent_path().string());
    std::string texturePath(parentPath + "/" + name);

    std::cout << "Texture path: " << texturePath.c_str() << std::endl;

    int height, width, numberOfChannels;
    unsigned char * data = stbi_load(texturePath.c_str(), &width, &height, &numberOfChannels, 0);

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D,textureID);

    //Aceita apenas imagens com 1, 3 ou 4 canais
    GLenum format;
        if (numberOfChannels == 1)
            format = GL_RED;
        else if (numberOfChannels == 3)
            format = GL_RGB;
        else if (numberOfChannels == 4)
            format = GL_RGBA;

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);

    return textureID;
}

////////////////////////////////////////////////////////////////////////
//                          MESH CLASS                                //
////////////////////////////////////////////////////////////////////////

void Mesh::setupMesh(){
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //Como structs em c++ são armazenadas sequencilamnete, isso é permitido e não há necessidade de enviar um atributo de cada vez
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);	
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
}

void Mesh::Draw(Shader & shader,size_t numberOfLights, GLfloat ** lightMatrix){
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    for(unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        string number;
        string name = textures[i].type;
        if(name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if(name == "texture_specular")
            number = std::to_string(specularNr++);
        
        shader.setInt((name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);

    LightManager * lightManager = LightManager::GetInstance();
    glm::vec3 ambient_light_vec = lightManager->getAmbientLight();
    double ambient_light[3];
    ambient_light[0] = ambient_light_vec.x;
    ambient_light[1] = ambient_light_vec.y;
    ambient_light[2] = ambient_light_vec.z;

    //O shader cria um array de tamanho MAX_LIGHT_NUM (128), e utiliza-se uma variável para iteração
    shader.setInt("light_num", numberOfLights);
    //Passando um array para o shader contendo as posições das luzes e suas cores
                       //Nome da variável  , array       , número de elementos
    shader.setVec3Array("light_positions", lightMatrix[0], numberOfLights);
    shader.setVec3Array("light_colors", lightMatrix[1], numberOfLights);
    shader.setVec3("ambient_light", ambient_light);

    //Chamada para o OpenGL desenhar
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

////////////////////////////////////////////////////////////////////////
//                         MODEL CLASS                                //
////////////////////////////////////////////////////////////////////////

vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
{
    vector<Texture> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        string textureName = str.C_Str();

        bool skip = false;

        if(loaded_textures.count(textureName) == 1){
            textures.push_back(loaded_textures[textureName]);
            skip = true;
        }
        if(!skip){
        Texture texture;
            texture.id = TextureFromFile(str.C_Str());
            texture.type = typeName;
            texture.name = textureName;
            textures.push_back(texture);
            loaded_textures.insert(make_pair(texture.name, texture));
        }
    }
    return textures;
}


Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene){
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;

    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        //Populando os structs Vertex do Mesh
        vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        if(mesh->mTextureCoords[0]){
            vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        }
        else vertex.TexCoords = glm::vec2(.0f, .0f);
        vertices.push_back(vertex);
    }

    //Populando vetor de índices do mesh
    for(unsigned int i = 0; i < mesh->mNumFaces; i ++){
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    if(mesh->mMaterialIndex >= 0)
    {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        //O tipo da aiTexture da textura é enviado para a função para acessar o array daquele tipo de textura
        vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");

        //Coloca todos os elementos em diffuse maps no vetor de texturas
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        //Repetindo para texturas speculars
        vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }  

    return Mesh(vertices, indices, textures);
}

void Model::processNode(aiNode *node, const aiScene *scene){
    //Processar todos os meshes presentes no nó antes de seguir para o próximo
    for(int i = 0; i < node->mNumMeshes; i++){
        //node->mMeshes contém índices para um determinado mesh na scene
        aiMesh * mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    //Recursivamente explorar cada nó filho
    for(int i = 0; i < node->mNumChildren; i++){
        processNode(node->mChildren[i], scene);
    }
}

void Model::loadModel(string path){
    Assimp::Importer importer;
    const aiScene * scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
        {
            cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
            return;
        }
    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void Model::Draw(Shader & shader){
    LightManager * lightManager = LightManager::GetInstance();
    GLfloat ** lightMatrix = lightManager->getLights();
    size_t numberOfLights = lightManager->getLightNum();
    for(int i = 0; i < meshes.size(); i++){
        meshes[i].Draw(shader, numberOfLights, lightMatrix);
    }
}