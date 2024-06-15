#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Mouse.hpp"

class Camera{
    private:
    //Inicializa um pouco distante da origam
    glm::vec3 position = glm::vec3(.0f, .0f, 10.0f);
    //A câmera ficará olhando para um ponto
    glm::vec3 point = glm::vec3(.0f,.0f,.0f);
    //Direção de cima da câmera
    glm::vec3 up;

    //Inicializa a câmera na posição inicial, olhando para a origem
    glm::mat4 view;

    //A câmera precisa do input do mouse para se mover
    Mouse mouse;

    //Capta o input do mouse
    void processMouseInput();

    void Rotate(double dx, double dy);

    void Move(double dx, double dy);

    public:

    //Criando a Câmera com valores padrões e atribuindo o mouse
    Camera(Mouse mouse): mouse(mouse){
        //Olhando em direção ao ponto (na verdade esse vetor aponta o oposto da direção em que a câmera está olhando)
        glm::vec3 direction = glm::normalize(position - point);
        //Encontrando a direção direita (x positivo) da câmera utilizando seu o vetor apontando para cima
        glm::vec3 right = glm::normalize(glm::cross(glm::vec3(.0f,1.0f,.0f), direction));
        //Vetor apontando para cima
        up = glm::normalize(glm::cross(direction, right));
        //Inicializa a câmera na posição inicial, olhando para a origem
        view = glm::lookAt(position, point, up);
    }

    //Função que irá mover a câmera ao longo da execução do programa
    //Será chamado or uma super classe responsável por atualizar, por frame
    //Todos os objetos do sistema
    void Update();

    glm::mat4 getViewMat(){
        return glm::lookAt(position, point, up);
    }
};