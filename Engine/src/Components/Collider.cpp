#include "Collider.hpp"
#include "CollideableManager.hpp"

#include <iostream>

using namespace Engine;

Collider::Collider(float width, float height, float depth) 
 : Component("collider") {
    dimensions = glm::vec3(width, height, depth);
}

void Collider::start(){
    //register to some kind've scene manager(stores collideables)
    CollideableManager::addCollideable(this);
}

void Collider::onEnable(){
    CollideableManager::addCollideable(this);
}

void Collider::onDisable(){
    CollideableManager::removeCollideable(this);
}

void Collider::update(){
    //check for collisions from CollideableManager and notify GameObject
    std::vector<CollisionEvent> events = CollideableManager::getCollisions(this);   
    for(auto & e : events){
        //notify the parent object
	std::cout << "Collision with: " << e.obj->getName() << std::endl;
        break;
    } 
}

void Collider::collision_update(){
    Transform* objTransform = (Transform*)object->getComponent("transform");
    glm::mat4 m = objTransform->calcModelMatrix() * glm::scale(glm::mat4(1.0), dimensions);
    verts[0] = glm::vec3(m*glm::vec4(-1, -1, -1, 1));
    verts[1] = glm::vec3(m*glm::vec4(-1, -1,  1, 1));
    verts[2] = glm::vec3(m*glm::vec4( 1, -1, -1, 1));
    verts[3] = glm::vec3(m*glm::vec4( 1, -1,  1, 1));
    verts[4] = glm::vec3(m*glm::vec4(-1,  1, -1, 1));
    verts[5] = glm::vec3(m*glm::vec4(-1,  1,  1, 1));
    verts[6] = glm::vec3(m*glm::vec4( 1,  1, -1, 1));
    verts[7] = glm::vec3(m*glm::vec4( 1,  1,  1, 1));
}

float calcOverlap(float min1, float max1, float min2, float max2){
    if(min1 < min2){
        return max1 - min2; 
    } else {
        return max2 - min1;
    }
    //return min1 < max2 && min2 < max1;
}

std::vector<Contact> Collider::didCollide(Collider* o_col){
    std::vector<Contact> contacts;
    
    Transform* m_trans = (Transform*)object->getComponent("transform");
    Transform* o_trans = (Transform*)o_col->getObject()->getComponent("transform");
    
    bool collide = true;
    std::vector<glm::vec3> 
        m_axis{m_trans->getForward(), m_trans->getRight(), m_trans->getUp()};
    std::vector<glm::vec3> 
        o_axis{o_trans->getForward(), o_trans->getRight(), o_trans->getUp()};
    
    std::vector<glm::vec3> axis = std::vector<glm::vec3>{m_axis[0], m_axis[1], m_axis[2],
        (float)-1.0*o_axis[0], (float)-1.0*o_axis[1], (float)-1.0*o_axis[2]};

    //TODO: trim useless axis
    int i = 0;
    for(auto & m_a : m_axis){
        for(auto & o_a : o_axis){
            if(m_a != o_a){
                axis.push_back(glm::normalize(glm::cross(m_a, o_a)));
            } else {
                //we can trim said axis
                //axis.erase(axis.begin() + i);
            }
        }
        i++;
    }
    float minOverlap = 1000000;
    glm::vec3 minTran = glm::vec3(0, 0, 0);
    for(auto & a : axis){
        float m_max = glm::dot(verts[0], a);
        float m_min = glm::dot(verts[0], a);
        for(int i = 1; i < 8; i++){
            float d = glm::dot(verts[i], a);
            if(d < m_min){m_min = d;} 
            else if(d > m_max){m_max = d;} 
        }
        glm::vec3 o_verts[8]; 
        memcpy(o_verts, o_col->getVerts(), 8 * sizeof(glm::vec3));
        float o_max = glm::dot(o_verts[0], a);
        float o_min = glm::dot(o_verts[0], a);
        for(int i = 1; i < 8; i++){
            float d = glm::dot(o_verts[i], a);
            if(d < o_min){o_min = d;} 
            else if(d > o_max){o_max = d;} 
        }
        float s = calcOverlap(m_min, m_max, o_min, o_max);
        if(s < 0){
            collide = false;
            break;
        }
        if(s < minOverlap){
            minOverlap = s;
            minTran = a;
            if(o_min < m_min){
                minTran *= -1; 
            }
        }
	}
    if(collide){
        contacts.push_back(Contact(minTran, minOverlap));    
    }
	return contacts;
}
