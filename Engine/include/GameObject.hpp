#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>

namespace Engine {
class GameObject;
}

#include "Component.hpp"

namespace Engine {

class GameObject {
public:
    GameObject(std::string _name, bool _isActive);
    GameObject(std::string _name, bool _isActive, bool _created);
    ~GameObject();

    void attachComponent(Component* c);
    //not no detaching components, as a class won't be able to detach components
    Component* getComponent(std::string name);

    void addChild(GameObject* child); //important for adding kids
    void removeChild(GameObject* child); //this function should not be called by components (it will leave children floating)

    GameObject* getChild(int index);
    GameObject* getChild(std::string _name);

    void update();
    void lateUpdate();

    void render(glm::mat4 projection, glm::mat4 view);

    GameObject* clone(); //this is the primary method of instantiating a GameObject

    //TO DO: Figure out what properties should definetly be private
    bool created; //set if allocated

    std::vector<GameObject*> children;
    std::vector<Component*> components;
    
    GameObject* getParent(){return parent;}
    void setParent(GameObject* _parent);
    
    std::string getName(){return name;}
 
    bool getActive(){return isActive;} 
    void setActive(bool state);
 
private:
    GameObject* parent;
    std::string name;
    bool isActive;
};

}
