#include "FirstPersonController.hpp"

using namespace Engine;

FirstPersonController::FirstPersonController() :
    Component(std::string("firstperson-controller")), WindowEventSubscriber() {

    WindowEventSubscriber::subscribe(WindowEventManager::events::KEYBOARD_INPUT);
    WindowEventSubscriber::subscribe(WindowEventManager::events::CURSOR_POSITION);

    speed = 50;
    mouseSpeed = 0.3;

    lastTime = 0;

    rotate = vec2(0,0);
    move = vec3(0,0,0);

    translationVector = vec3(0,0,0);
    rotationVector = vec2(0,180);
}

void FirstPersonController::onKey(int key, int scancode, int action, int mods) {
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

void FirstPersonController::onCursorMove(double xpos, double ypos) {
    int width = ((Camera*)object->getComponent("camera"))->getWidth();
    int height = ((Camera*)object->getComponent("camera"))->getHeight();

    rotate = vec2(0,float(width/2-xpos));
    rotate += vec2(-float(height/2-ypos),0);

}

void FirstPersonController::calcTranslation(float dt){
    if (length(move) <= 0){ translationVector = vec3(0); return;}

    vec3 forward = ((Transform*)object->getComponent("transform"))->getForward();
    vec3 right = ((Transform*)object->getComponent("transform"))->getRight();

    translationVector = (move.x*right) + (move.y*vec3(0,1,0)) + (move.z*forward);
    translationVector = normalize(translationVector) * speed * dt;
}

void FirstPersonController::calcRotation(float dt){
    rotationVector += rotate * dt * mouseSpeed;

    if (rotationVector.x > 1.5){
        rotationVector.x = 1.5;
    }else if (rotationVector.x < -1.5){
        rotationVector.x = -1.5;
    }

    rotate = vec2(0,0);
}

void FirstPersonController::start(){
    lateUpdate();
}

void FirstPersonController::update(){
    float deltaTime = float(glfwGetTime()-lastTime);
    Transform* objTf = ((Transform*)object->getComponent("transform"));

    calcRotation(deltaTime);
    objTf->setRotation(rotationVector.x,rotationVector.y,0);

    calcTranslation(deltaTime);
    objTf->translate(translationVector.x,translationVector.y,translationVector.z);
	
	glm::vec3 p = objTf->calcGlobalPosition();
	//std::cout << "x: " << p.x << " y: " << p.y << " z: " << p.z << std::endl; 

    lastTime = glfwGetTime();
}
