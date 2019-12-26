#pragma once

#include "Collider.hpp"

namespace Engine {

struct CollisionEvent {
    CollisionEvent() 
     : obj(NULL), other(NULL), contacts(std::vector<Contact>()) {}
    
    CollisionEvent(GameObject* _obj, Collider* _other, std::vector<Contact> _contacts)
     : obj(_obj), other(_other), contacts(_contacts) {}
    
    GameObject* obj; 
    Collider* other;
    std::vector<Contact> contacts; 
};

class CollideableManager {
public:
    static void addCollideable(Collider* c);
    static void removeCollideable(Collider* c);
    
    static void update(); //gets all the collisions in the scene

    static std::vector<CollisionEvent> getCollisions(Collider* c);
private:
    struct dataNode {
    dataNode(Collider* _collider) 
     : collider(_collider), eventsSize(0){}
        Collider* collider;
        int eventsSize;
        CollisionEvent events[10]; 
    };
    static std::vector<dataNode> data;
};

}
