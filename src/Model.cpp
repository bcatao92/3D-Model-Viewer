#include "Model.hpp"
#include "Light.hpp"
#include <filesystem>

GLuint TextureFromFile(const char* name){
    GLuint textureID;

    //Essa função assume que a textura esteja no diretório do binário
    std::filesystem::path path = std::filesystem::current_path();
    std::string parentPath(path.parent_path().string());
    std::string texturePath(parentPath + name);

    int height, width, numberOfChannels;
    unsigned char * data = stbi_load(texturePath.c_str(), &width, &height, &numberOfChannels, 0);

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D,textureID);

    //Aceita apenas imagens com 3 ou 4 canais
    if(numberOfChannels == 3)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    else
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glBindTexture(GL_TEXTURE_2D, 0);

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

void Mesh::Draw(Shader & shader, glm::vec3 * LightPositions, glm::vec3 * LightColors){
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

        shader.setInt(("material." + name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);

    

    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

////////////////////////////////////////////////////////////////////////
//                         MODEL CLASS                                //
////////////////////////////////////////////////////////////////////////

vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
{
    vector<Texture> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        Texture texture;
        texture.id = TextureFromFile(str.C_Str());
        texture.type = typeName;
        //texture.path = str;
        textures.push_back(texture);
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
            vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][1].x, mesh->mTextureCoords[0][i].y);
        }
        else vertex.TexCoords = glm::vec2(.0f, .0f);
        vertices.push_back(vertex);
    }

    //Populando vetor de índices do mesh
    for(int i = 0; i < mesh->mNumFaces; i ++){
        for(int j = 0; mesh->mFaces[i].mNumIndices; j++){
            indices.push_back(mesh->mFaces[i].mIndices[j]);
        }
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
    glm::vec3 * lightPositions = lightManager->getLightPositions();
    glm::vec3 * lightColors = lightManager->getLightColors();
    for(int i = 0; i < meshes.size(); i++){
        meshes[i].Draw(shader, lightPositions, lightColors);
    }
}