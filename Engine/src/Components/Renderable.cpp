#include "Renderable.hpp"

#include <iostream>

#include "Transform.hpp"

using namespace Engine;

Renderable::Renderable() 
 : Component("renderable") {
    isShown = true;
}

Renderable::Renderable(VAO* _vao, ShaderProgram* _program) : Component("renderable") {
    isShown = true;
    vao = _vao;
    shaderProgram = _program;
}

void Renderable::setVAO(VAO* _vao){
    vao = _vao;
}
/*
void Renderable::createShader(const char* vPath, const char* fPath){
    shaderProgram = new ShaderProgram(vPath,fPath);
}
*/
void Renderable::setShaderProgram(ShaderProgram* program){
    shaderProgram = program;
}

void Renderable::render(glm::mat4 projectionMatrix, glm::mat4 viewMatrix){
    if(isShown){
        //std::cout << "Rendering: " << object->getName() << std::endl;
        glm::mat4 newModelMatrix = ((Transform*)object->getComponent("transform"))->calcModelMatrix();
        shaderProgram->start();
        if(bones.size() > 0){
            shaderProgram->setUniformMat4Array(bones, "boneTransforms");     
        }
        shaderProgram->setUniformVec3(color, "c");
        shaderProgram->setUniformMat4(newModelMatrix,"M");
        shaderProgram->setUniformMat4(viewMatrix,"V");
        shaderProgram->setUniformMat4(projectionMatrix,"P");
        vao->draw();
    }
}

void Renderable::setColor(float r, float g, float b){
    color = glm::vec3(r, g, b);
}
