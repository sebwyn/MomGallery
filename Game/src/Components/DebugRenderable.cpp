#include "DebugRenderable.hpp"
#include "Time.hpp"

bool DebugRenderable::first = true;
Engine::ShaderProgram* DebugRenderable::program;

DebugRenderable::DebugRenderable(GLuint _texture, Engine::ShaderProgram* _program)
 : texture(_texture) {
    if(first){
        first = false;
        //program = _program;
        program = new Engine::ShaderProgram("../Shaders/debugTexture.vs", "../Shaders/debugTexture.fs");
    }
    vao = new Engine::VAO();

    glGenTextures(1, &tex);
    
    //8 unsigned int padding
    /*unsigned int texData[16*16];
    memset(texData, 0xFF, (16*16)*sizeof(unsigned int));

    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 16, 16, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    */
    
}

DebugRenderable::~DebugRenderable(){
    delete vao;    
}

void DebugRenderable::render(glm::mat4 projectionMatrix, glm::mat4 viewMatrix){
    float timeSinceStart = Engine::Time::getTime();

    program->start();    
    program->setUniformFloat( fmod(timeSinceStart, 1.0), "progression");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D, texture);
    
    //a VAO must be bound in order to draw
    vao->bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    vao->unbind();
}
