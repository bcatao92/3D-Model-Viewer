#pragma once
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include "Mouse.hpp"

class Camera{
    private:
    //Inicializa um pouco distante da origam
    glm::vec3 position = glm::vec3(.0f, .0f, 10.0f);
    //A câmera ficará olhando para um ponto
    glm::vec3 point = glm::vec3(.0f,.0f,.0f);
    //Direção de cima da câmera
    glm::vec3 worldUp = glm::vec3(0.f,1.f,0.f);
    //Direita é necessária para determinar o vetor up
    glm::vec3 right;
    //Direção de cima da câmera
    glm::vec3 up;
    //Direção para qual a câmera está olhando (o ponto)
    glm::vec3 direction = glm::vec3(position-point);
    //Yaw e pitch
    double yaw = -90., pitch = 0.;

    //Raio entre a câmera e o ponto, usado para rotação
    double distance = glm::length(position-point);

    //Inicializa a câmera na posição inicial, olhando para a origem
    glm::mat4 view;

    //A câmera precisa do input do mouse para se mover
    Mouse * mouse;

    //Capta o input do mouse
    void processMouseInput(double deltaTime);

    void Rotate(double dx, double dy);

    void Move(double dx, double dy);

    public:

    //Criando a Câmera com valores padrões e atribuindo o mouse
    Camera(Mouse* mouse){
        //Olhando em direção ao ponto (na verdade esse vetor aponta o oposto da direção em que a câmera está olhando)
        direction = glm::normalize(position - point);
        //Encontrando a direção direita (x positivo) da câmera utilizando seu o vetor apontando para cima
        right = glm::normalize(glm::cross(worldUp, direction));
        //Vetor apontando para cima
        up = glm::normalize(glm::cross(direction, right));
        //Inicializa a câmera na posição inicial, olhando para a origem
        view = glm::lookAt(position, point, up);

        this->mouse = mouse;
    }

    //Função que irá mover a câmera ao longo da execução do programa
    //Será chamado or uma super classe responsável por atualizar, por frame
    //Todos os objetos do sistema
    void Update(double deltaTime);

    glm::mat4 getViewMat(){
        return glm::lookAt(position, point, up);
    }

    glm::vec3 getPosition(){
        return position;
    }
    
};