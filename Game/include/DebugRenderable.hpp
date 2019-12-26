#pragma once

#include "ShaderProgram.hpp"
#include "Renderable.hpp"

class DebugRenderable : public Engine::Renderable {
private:
    static bool first;
    static Engine::ShaderProgram* program;
    Engine::VAO* vao;
    GLuint texture;
    GLuint tex;
public:
    DebugRenderable(GLuint _texture, Engine::ShaderProgram* program);
    virtual ~DebugRenderable();
    
    virtual void render(glm::mat4 projectionMatrix, glm::mat4 viewMatrix); 
};
