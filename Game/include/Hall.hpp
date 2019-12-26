#pragma once

#include "VAO.hpp"
#include "ShaderProgram.hpp"
#include "Component.hpp"

#include <glm/glm.hpp>

class Hall : public Engine::Component {
public:
    static float width;
    static float length;
    static float height;
    
    static Engine::ShaderProgram* program;
    static Engine::VAO* mesh;
     
    float x, y, z;
    int type;

    Hall(float _x, float _y, float _z, bool _facesZ, int _type);

    virtual void start();

    bool doesCollide(Engine::GameObject* obj);
private:
    bool facesZ;
};
