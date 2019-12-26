#include "VAO.hpp"

#include <iostream>

using namespace Engine;

VAO::VAO(){
    glGenVertexArrays(1, &vaoID);
    ebo = 0;
    vertexCount = 0;
}

VAO::~VAO(){
    glDeleteVertexArrays(1,&vaoID);

    for(int i = 0; i < vbos.size(); i++){
        glDeleteBuffers(1,&vbos[i]);
    }
}

void VAO::addGLFloatBuffer(int attrib, GLuint buffer, int entries, int entrySize){
    bind();
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glVertexAttribPointer(
        attrib, 
        entrySize,
        GL_FLOAT,
        GL_FALSE,
        0,
        (void*)0
    );
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    unbind();

    vbos.push_back(buffer);
    attribs.push_back(attrib);
}

void VAO::addGLIntBuffer(int attrib, GLuint buffer, int entries, int entrySize){
    bind();
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glVertexAttribIPointer(
        attrib,
        entrySize,
        GL_INT,
        0,
        (void*)0
    );
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    unbind();

    vbos.push_back(buffer);
    attribs.push_back(attrib);
}

void VAO::addGLElementBuffer(GLuint buffer, int entries){
    vertexCount = entries;
    
    bind();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
    unbind();

    ebo = buffer;
}

void VAO::addFloatBuffer(int attrib, GLfloat* data, int entries, int entrySize){

    bind();
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, entries*entrySize*sizeof(GLfloat), data, GL_STATIC_DRAW);
    glVertexAttribPointer(
        attrib,
        entrySize,
        GL_FLOAT,
        GL_FALSE,
        0,
        (void*)0
    );
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    unbind();

    if (attrib == 0) vertexCount = entries;

    vbos.push_back(vbo);
    attribs.push_back(attrib);
}

void VAO::addUIntBuffer(int attrib, GLuint* data, int entries, int entrySize){

    bind();
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, entries*entrySize*sizeof(GLuint), data, GL_STATIC_DRAW);
    glVertexAttribIPointer(
        attrib,
        entrySize,
        GL_UNSIGNED_INT,
        0,
        (void*)0
    );
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    unbind();

    if (attrib == 0 && !(ebo > 0)) vertexCount = entries;

    vbos.push_back(vbo);
    attribs.push_back(attrib);
}

void VAO::addElementBuffer(GLshort* data, int entries){

    vertexCount = entries;

    bind();
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, entries*sizeof(GLshort), data, GL_STATIC_DRAW);
    unbind();
}

void VAO::draw() const{
    bind();
    enableAttribs();
    if (ebo > 0) {
        std::cout << "What the fuck" << std::endl;
        glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_SHORT, 0);
    }else{
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    }
    disableAttribs();
    unbind();
}

void VAO::enableAttribs() const{
    for(int i = 0; i < attribs.size(); i++){
        glEnableVertexAttribArray(attribs[i]);
    }
}

void VAO::disableAttribs() const{
    for(int i = 0; i < attribs.size(); i++){
        glDisableVertexAttribArray(attribs[i]);
    }
}

void VAO::bind() const{
    glBindVertexArray(vaoID);
}

void VAO::unbind() const{
    glBindVertexArray(0);
}
