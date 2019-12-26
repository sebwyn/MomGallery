#pragma once

//opengl includes
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

class Texture {
public:
    //type is GL_TEXTURE_*
    //format is the textures internal format
    //
    Texture(GLenum _type, GLenum format, glm::ivec3 _dimensions, void* data);
    
    void use(GLenum texUnit);
private:
    GLuint id;

    GLenum type;
    glm::ivec3 dimensions;
};
