#pragma once

#include "ShaderProgram.hpp"
#include "VAO.hpp"

#include "Component.hpp"

#include <vector>
#include <glm/glm.hpp>

namespace Engine {

class Renderable : public Component {
public:
    Renderable();
    Renderable(VAO* _vao, ShaderProgram* _program);
    virtual ~Renderable(){}

    void setVAO(VAO* _vao);
    //void createShader(const char* vPath, const char* fPath);
    void setShaderProgram(ShaderProgram* program);

    virtual void render(glm::mat4 projectionMatrix, glm::mat4 viewMatrix);

    inline bool getShown() const {return isShown;}
    void setShown(bool state){isShown = state;}

    void setColor(float r, float g, float b);
	inline void setBoneTransforms(std::vector<glm::mat4> _bones){bones = _bones;}
private:
    bool isShown;

    //rendering vars (later this will probably be a material)
    glm::vec3 color = glm::vec3(1, 1, 1);
	//if the object has a skeleton here are the transforms
	std::vector<glm::mat4> bones = std::vector<glm::mat4>(0);

    VAO* vao;
    ShaderProgram* shaderProgram;
};

}
