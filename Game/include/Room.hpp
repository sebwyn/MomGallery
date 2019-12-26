#pragma once

#include "VAO.hpp"
#include "ShaderProgram.hpp"
#include "Component.hpp"

class Room : public Engine::Component {
public:
    static float width;
    static float length;
    static float height;
    
    static Engine::ShaderProgram* program;
    static Engine::VAO* mesh;
    
    //references galleries halls (in theory sorted by type)
    //rooms sorted by type
    int type;

    float x, y, z;
        
    Room(float _x, float _y, float _z, int _type);

    virtual void start();

    void genHalls();

    bool doesCollide(Engine::GameObject* obj);
};
