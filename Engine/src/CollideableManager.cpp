#include "CollideableManager.hpp"

#include <iostream>

using namespace Engine;

std::vector<CollideableManager::dataNode> CollideableManager::data;

void CollideableManager::addCollideable(Collider* c){
    data.push_back(dataNode(c));
}

void CollideableManager::removeCollideable(Collider* c){
    int index;
    bool found = false;
    for(int i = 0; i < data.size(); i++){
 	if(data[i].collider == c){
 	    index = i;
	    found = true;
	    break; 	    
   	}
    }
    if(found) data.erase(data.begin() + index); 
}

void CollideableManager::update(){
    for(int i = 0; i < data.size(); i++){
        data[i].eventsSize = 0; 
        data[i].collider->collision_update();
    }
    for(int i = 0; i < data.size(); i++){
        for(int c = i + 1; c < data.size(); c++){
            std::vector<Contact> contacts = data[i].collider->didCollide(data[c].collider);
            std::vector<Contact> oppContacts;
            for(auto & c : contacts){
                oppContacts.push_back(Contact((float)-1.0*c.normal, c.penDepth)); 
            }
            if(contacts.size() != 0){
                if(data[i].eventsSize != 10 && data[c].eventsSize != 10){
                    data[i].events[data[i].eventsSize] = CollisionEvent(data[c].collider->getObject(), data[c].collider, contacts);
                    data[i].eventsSize++;
                    data[c].events[data[c].eventsSize] = CollisionEvent(data[i].collider->getObject(), data[i].collider, contacts);
                    data[c].eventsSize++;
                }	
            } 
        }
    }
}

std::vector<CollisionEvent> CollideableManager::getCollisions(Collider* c){
    std::vector<CollisionEvent> events = std::vector<CollisionEvent>(0); 
    for(auto & node : data){
        if(node.collider == c){
            events.assign(&(node.events[0]), &(node.events[node.eventsSize])); 
        }
    }
    return events;
}
