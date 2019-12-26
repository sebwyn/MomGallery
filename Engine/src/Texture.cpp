#include "Texture.hpp"

Texture::Texture(GLenum _type, GLenum format, glm::ivec3 _dimensions, void* data)
 : type(_type), dimensions(_dimensions){
}
