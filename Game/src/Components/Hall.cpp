#include "Hall.hpp"

#include "Renderable.hpp"
#include "Transform.hpp"

#include <iostream>

#define PI 3.141592

float Hall::width = 0;
float Hall::height = 0;
float Hall::length = 0;
Engine::ShaderProgram* Hall::program = 0;
Engine::VAO* Hall::mesh = 0;

Hall::Hall(float _x, float _y, float _z, bool _facesZ, int _type)
 : Component("hall"), x(_x), y(_y), z(_z), facesZ(_facesZ), type(_type){
    std::cout << "making hall" << std::endl;
}

void Hall::start(){
    if(mesh && program){
        object->attachComponent(new Engine::Renderable(mesh, program));
    } else {
        std::cout << "Mesh or Vao has not been initialized" << std::endl;
    }

    Engine::Transform* transform = (Engine::Transform*)object->getComponent("transform");
    //std::cout << "starting " << x << " " << y << " " << z << std::endl;  
    transform->setPosition(x, y, z);
    if(!facesZ){
    	transform->setRotation(0, PI/2, 0);
    }	
}

bool Hall::doesCollide(Engine::GameObject* obj){
    Engine::Transform* mTrans = (Engine::Transform*)object->getComponent("transform");
    glm::vec3 mPos = mTrans->calcGlobalPosition();
    Engine::Transform* oTrans = (Engine::Transform*)obj->getComponent("transform");
    glm::vec3 oPos = oTrans->calcGlobalPosition();
    glm::vec3 min, max;
    //make slightly smaller than actual dimensions
    float w = width/2;
    float h = height/2;
    float l = length/2;
    if(facesZ){
        //length maps to z
        max = mPos + glm::vec3(w, h, l);
        min = mPos - glm::vec3(w, h, l);
    } else {
        //length maps to x
        max = mPos + glm::vec3(l, h, w);
        min = mPos - glm::vec3(l, h, w);
    }
    if(min.x < oPos.x && oPos.x < max.x){
        if(min.z < oPos.z && oPos.z < max.z){
            std::cout << "Collided With Hall" << std::endl;
            std::cout << "Max: " << max.x << " " << max.z <<std::endl;
            std::cout << "Min: " << min.x << " " << min.z <<std::endl;
            std::cout << "Camera: " << oPos.x << " " << oPos.z << std::endl;
            std::cout << "Hall: " << mPos.x << " " << mPos.z << std::endl;
            return true;
        }
    }
    return false;
}
