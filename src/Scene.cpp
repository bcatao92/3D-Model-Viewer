#include "Scene.hpp"

using namespace std;
using namespace glm;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);  

bool Scene::gladLoaded = false;
bool Scene::glfwLoaded = false;


Scene::Scene(const char * title, int screenWidth, int screenHeight){
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    projection = glm::perspective(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);

    if(glfwLoaded == false){
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }

    window = glfwCreateWindow(screenWidth, screenHeight, title, NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        throw std::exception();
    }
    glfwMakeContextCurrent(window);

    if(gladLoaded == false){
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            throw std::exception();
        }
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); 
    
    glViewport(0, 0, screenWidth, screenHeight);

    glEnable(GL_DEPTH_TEST);

    mouse = new Mouse(window);
    camera = new Camera(mouse);
}

void Scene::Update(){
    double time = glfwGetTime();
    double deltaTime = time - lastTime;
    lastTime = time;

    camera->Update(deltaTime);
    mouse->Update();
}

void Scene::Draw(){
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for(Model model : models){
        Shader * modelShader = model.shader;
        
        modelShader->use();
        mat4 view = camera->getViewMat();
        modelShader->setMat4("projection", projection);
        modelShader->setMat4("view", view);

        vec3 scale = model.getScale();
        modelMatrix = translate(modelMatrix, model.position); // translate it down so it's at the center of the scene
        modelMatrix = glm::scale(modelMatrix, scale);	// it's a bit too big for our scene, so scale it down
        modelShader->setMat4("model", modelMatrix);

        GLfloat ** lightMatrix = lightManager.getLights();
        vec3 ambientLight = lightManager.getAmbientLight();
        size_t lightNum = lightManager.getLightNum();

        //Luz ambiente
        modelShader->setVec3("ambient_light", ambientLight);
        //O shader cria um array de tamanho MAX_LIGHT_NUM (128), e utiliza-se uma variável para iteração
        modelShader->setInt("light_num", lightNum);
        //Passando um array para o shader contendo as posições das luzes e suas cores
                        //Nome da variável  , array       , número de elementos
        modelShader->setVec3Array("light_positions", lightMatrix[0], lightNum);
        modelShader->setVec3Array("light_colors", lightMatrix[1], lightNum);

        //Passando a posição da câmera para o shader, para o cálculo da reflexão espectular
        vec3 cameraPosition = camera->getPosition();
        modelShader->setVec3("acameraPos", cameraPosition);

        model.Draw();
    }

    glfwSwapBuffers(window);
    glfwPollEvents();  
}

void Scene::Start(){
    while(!glfwWindowShouldClose(window)){
        Update();
        Draw();
    }
    glfwTerminate();
}