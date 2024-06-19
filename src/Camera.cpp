#include "Camera.hpp"
#include <iostream>

//Retorna o ângulo de um triângulo isóceles de lado igual a distance e lado oposto igual a displacement
//EM RADIANOS
double getAngle(double distance, double displacement){
    double distanceSquared2 = 2*distance*distance;
    double dxsquared = displacement*displacement;
    double signal = (displacement >= 0) ? 1.: -1.;
    double cos = 1-(dxsquared/distanceSquared2);
    return glm::acos(cos)*signal;
}

using namespace std;

void Camera::Move(double dx, double dy){
    double xscalar = dx;
    double yscalar = -dy;
    point += glm::vec3(up.x*yscalar,up.y*yscalar,up.z*yscalar);
    point += glm::vec3(right.x*xscalar,right.y*xscalar,right.z*xscalar);
}

void Camera::Rotate(double dx, double dy){
    //É necessário determinar a direção para a qual o mouse se moveu para encontrar
    //o eixo sobre o qual rotacionar utilizando produto vetorial
    double xangle = getAngle(distance ,dx);
    double yangle = getAngle(distance, dy);

    yaw += glm::degrees(xangle);
    pitch += glm::degrees(yangle);
}

void Camera::processMouseInput(double deltaTime){
    bool left, right;
    double dx, dy;
    mouse->getButtonsPressed(left, right);
    mouse->getDistance(dx,dy);
    dx *= deltaTime*mouse->sensitivity;
    dy *= deltaTime*mouse->sensitivity;
    if(dx != 0. && dy != 0){
        if(left)
            Rotate(dx,dy);
        if(right)
            Move(dx,dy);
    }
}

void Camera::Update(double deltaTime){
    //Encontrando a direção em que o ponto está olhando
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    //Direção em que a câmera deve olhar para o ponto
    direction = -glm::normalize(direction);
    //Escalando a direção pela distância que deve estar do ponto
    //esssa é a posição da câmera relativa ao ponto
    glm::vec3 scaledDirection = direction*(float)distance;
    //Encontrando a posição absoluta da câmera
    position = glm::vec3(scaledDirection+point);
    //Quando o modelo rotaciona mais de 90 graus no eixo vertical
    //esse produto cruzado se inverte, fazendo com que o modelo
    //seja espelhado com relação ao eixo x.
    right = glm::normalize(glm::cross(direction, worldUp));
    up = glm::normalize(glm::cross(right,direction));
    distance += Mouse::scrollOffset*deltaTime*50.;
    processMouseInput(deltaTime);
}