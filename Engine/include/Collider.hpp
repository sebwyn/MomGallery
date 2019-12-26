#pragma once

#include "Component.hpp"
#include "Transform.hpp"

#include <vector>
#include <glm/glm.hpp>

namespace Engine {

class Collider;

struct Contact {
    Contact(glm::vec3 norm, float depth) 
     : normal(norm), penDepth(depth) {}
    glm::vec3 normal;
    float penDepth;
};

//class that encapsulates communication between 

class Collider : public Component {
public:
    Collider(float width, float height, float depth);
    ~Collider(){return;}

    virtual void start();
    virtual void update();
    
    virtual void onEnable();
    virtual void onDisable();

    void collision_update();
    std::vector<Contact> didCollide(Collider* obj);

    inline GameObject* getObject() const {return object;}
	inline const glm::vec3* getVerts() const {return &verts[0];}
private:
    glm::vec3 dimensions;
    glm::vec3 verts[8];
};

}
