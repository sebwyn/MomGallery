#include "Camera.hpp"

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

#include "Transform.hpp"

using namespace Engine;

Camera::Camera(float _width, float _height)
 : Component(std::string("camera")) {
    FOV = 90;
    width = _width;
    height = _height;
    lookAtState = lookAtMode::DISABLED;
    viewDirection = glm::vec3(0,0,1);
}

void Camera::setFOV(float fov){
    FOV = fov;
}

void Camera::updateAspect(float _width, float _height){
    width = _width;
    height = _height;
}

void Camera::start(){
    //lateUpdate();
}

void Camera::lateUpdate(){
    glm::vec3 target;
    glm::vec3 position = ((Transform*)object->getComponent("transform"))->calcGlobalPosition();
    switch (lookAtState){
        case lookAtMode::POINT:
            viewDirection = glm::normalize(targetPoint-position);
            break;
        case lookAtMode::OBJECT:
            target = ((Transform*)targetObject->getComponent("transform"))->calcGlobalPosition();
            viewDirection = glm::normalize(target-position);
            break;
        case lookAtMode::DISABLED:
            viewDirection = ((Transform*)object->getComponent("transform"))->getForward();
            break;
    }
    calculateMatrices();
}

void Camera::lookAt(GameObject* object){
    lookAtState = lookAtMode::OBJECT;
    targetObject = object;
}

void Camera::lookAt(float x, float y, float z){
    lookAtState = lookAtMode::POINT;
    targetPoint = glm::vec3(x,y,z);
}

void Camera::disableLookAt(){
    lookAtState = lookAtMode::DISABLED;
}

void Camera::calculateMatrices(){
    glm::vec3 position = ((Transform*)object->getComponent("transform"))->calcGlobalPosition();
    viewMatrix = glm::lookAt(position,
                                    position+viewDirection,
                                    glm::vec3(0,1,0));

    projectionMatrix = glm::perspective(glm::radians(FOV),
                                         width / height,
                                         0.1f,
                                         10000.0f);
}
