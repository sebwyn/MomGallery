#include "Component.hpp"

using namespace Engine;

Component::Component(std::string _name){
    isEnabled = true;
    name = _name;
}

void Component::setEnabled(bool state){
    if(isEnabled != state){
  	isEnabled = state;
        if(state == true){
            onEnable();
        } else {
  	    onDisable();
        }
    }
}
