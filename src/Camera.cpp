#include "Camera.hpp"
#include <GLFW/glfw3.h>
#include <glm/gtc/epsilon.hpp>
#include <iostream>

double getAngle(double distance, double displacement){
    double distanceSquared2 = 2*distance*distance;
    double dxsquared = displacement*displacement;
    double signal = (displacement >= 0) ? 1.: -1.;
    double cos = 1-(dxsquared/distanceSquared2);
    return glm::acos(cos)*signal;
}

using namespace std;

void Camera::Move(double dx, double dy){
    view = glm::translate(view, glm::vec3(dx,dy, 0.f));
}

void Camera::Rotate(double dx, double dy){
    //É necessário determinar a direção para a qual o mouse se moveu para encontrar
    //o eixo sobre o qual rotacionar utilizando produto vetorial
    double xangle = getAngle(distance ,dx);
    double yangle = getAngle(distance, dy);

    // cout << "Angle X: " << xangle << endl;
    // cout << "Angle Y: " << yangle << endl;

    glm::mat4 model = glm::mat4(1.0);
    glm::mat4 rotationx = glm::rotate(model, (float)xangle, up);
    position = glm::vec4(position.x,position.y,position.z, 0.0f)*rotationx;
    

    glm::mat4 rotationy = glm::rotate(model, (float)yangle, right);
    position = glm::vec4(position.x,position.y,position.z, 0.0f)*rotationy;

    //cout << "Position: " << "X: " << position.x << "Y: " << position.y << "Z: " << position.z << endl;
    // position.x = result.x;
    // position.y = result.y;
    // position.z = result.z;

    glm::vec3 front = glm::vec3(position - point);
    front = glm::normalize(front);
}

void Camera::processMouseInput(){
    mouse.Update();
    bool left;
    bool right;
    double dx;
    double dy;
    mouse.getButtonsPressed(left, right);
    mouse.getDistance(dx,dy);
    if(dx != 0. && dy != 0){
        if(left)
        if(right)
            Move(dx,dy);
    }
}

void Camera::Update(){
    glm::vec3 front = glm::normalize(glm::vec3(point - position));
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right,front));
    cout << "front: " << " X: " << front.x << " Y: " << front.y << " Z: " << front.z << endl;
    cout << "UP: " << " X: " << up.x << " Y: " << up.y << " Z: " << up.z << endl;
    Rotate(0., 5.*10./600.);
}