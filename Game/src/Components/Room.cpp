#include "Room.hpp"

#include "Hall.hpp"
#include "Renderable.hpp"
#include "Transform.hpp"

#include <iostream>

float Room::width = 0;
float Room::height = 0;
float Room::length = 0;
Engine::ShaderProgram* Room::program = 0;
Engine::VAO* Room::mesh = 0;

Room::Room(float _x, float _y, float _z, int _type)
 : Component("room"), x(_x), y(_y), z(_z), type(_type) {
    //std::cout << "Making Room" << std::endl;
}

void Room::start(){
    if(mesh && program){
        object->attachComponent(new Engine::Renderable(mesh, program));
    } else {
        std::cout << "Mesh or Vao has not been initialized" << std::endl;
    }

    Engine::Transform* transform = (Engine::Transform*)object->getComponent("transform");
    transform->setPosition(x, y, z);
}


bool Room::doesCollide(Engine::GameObject* obj){
    Engine::Transform* mTrans = (Engine::Transform*)object->getComponent("transform");
    glm::vec3 mPos = mTrans->calcGlobalPosition();
    Engine::Transform* oTrans = (Engine::Transform*)obj->getComponent("transform");
    glm::vec3 oPos = oTrans->getPosition();
    //make sizes slightly smaller than actual model
    float w = width/2;
    float h = height/2;
    float l = length/2;
    glm::vec3 max = mPos + glm::vec3(w, h, l);
    glm::vec3 min = mPos - glm::vec3(w, h, l);
    if(min.x < oPos.x && oPos.x < max.x){
        if(min.z < oPos.z && oPos.z < max.z){
            return true;
            //std::cout << "Collided With Room" << std::endl;
        }
    }
    return false;
}
