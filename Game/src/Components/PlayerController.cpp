#include "PlayerController.hpp"

#define PI 3.141592

#include <algorithm>

PlayerController::PlayerController() :
    Component(std::string("firstperson-controller")), WindowEventSubscriber() {

    WindowEventSubscriber::subscribe(WindowEventManager::events::KEYBOARD_INPUT);
    WindowEventSubscriber::subscribe(WindowEventManager::events::CURSOR_POSITION);

    speed = 10;
    mouseSpeed = 0.3;

    lastTime = 0;

    rotate = vec2(0,0);
    move = vec3(0,0,0);

    translationVector = vec3(0,0,0);
    rotationVector = vec2(0,PI);
}

void PlayerController::onKey(int key, int scancode, int action, int mods) {
    vec3 fv = ((Transform*)object->getComponent("transform"))->getForward();
    vec3 rv = ((Transform*)object->getComponent("transform"))->getRight();
    if (action == GLFW_PRESS){
        switch(key){
            case GLFW_KEY_W:
                move += vec3(0, 0, 1);
                break;
            case GLFW_KEY_A:
                move -= vec3(1, 0, 0);
                break;
            case GLFW_KEY_S:
                move -= vec3(0, 0, 1);
                break;
            case GLFW_KEY_D:
                move += vec3(1, 0, 0);
                break;
            case GLFW_KEY_SPACE:
                move += vec3(0,1,0);
                break;
            case GLFW_KEY_LEFT_SHIFT:
                move -= vec3(0,1,0);
                break;
            default:
                break;
        }
    }else if (action == GLFW_RELEASE){
        switch(key){
            case GLFW_KEY_W:
                move -= vec3(0, 0, 1);
                break;
            case GLFW_KEY_A:
                move += vec3(1, 0, 0);
                break;
            case GLFW_KEY_S:
                move += vec3(0, 0, 1);
                break;
            case GLFW_KEY_D:
                move -= vec3(1, 0, 0);
                break;
            case GLFW_KEY_SPACE:
                move -= vec3(0,1,0);
                break;
            case GLFW_KEY_LEFT_SHIFT:
                move += vec3(0,1,0);
                break;
            default:
                break;
        }
    }
}

void PlayerController::onCursorMove(double xpos, double ypos) {
    int width = ((Camera*)camera->getComponent("camera"))->getWidth();
    int height = ((Camera*)camera->getComponent("camera"))->getHeight();

    rotate = vec2(0,float(width/2-xpos));
    rotate += vec2(-float(height/2-ypos),0);
}

void PlayerController::calcTranslation(float dt){
    if (length(move) <= 0){ translationVector = vec3(0); return;}

    vec3 forward = ((Transform*)object->getComponent("transform"))->getForward();
    vec3 right = ((Transform*)object->getComponent("transform"))->getRight();

    translationVector = (move.x*right) + (move.y*vec3(0,1,0)) + (move.z*forward);
    translationVector = normalize(translationVector) * speed * dt;
}

void PlayerController::calcRotation(float dt){
    rotationVector += rotate * dt * mouseSpeed;
    rotationVector.x=clamp((float)rotationVector.x, (float)-PI/(float)8.0, (float)PI/(float)8.0);
    /*
    if (rotationVector.x > 1.5){
        rotationVector.x = 1.5;
    }else if (rotationVector.x < -1.5){
        rotationVector.x = -1.5;
    }
    */
    rotate = vec2(0,0);
}

void PlayerController::start(){
    head = object->getChild("head");
    camera = head->getChild("camera");
}

void PlayerController::update(){
    float deltaTime = float(glfwGetTime()-lastTime);
    Transform* objTf = (Transform*)object->getComponent("transform");
    Transform* headTF = (Transform*)head->getComponent("transform");

    calcRotation(deltaTime);
    objTf->setRotation(0,rotationVector.y,0);
    headTF->setRotation(rotationVector.x,0,0);

    calcTranslation(deltaTime);
    objTf->translate(translationVector.x,0,translationVector.z);

    lastTime = glfwGetTime();
}
