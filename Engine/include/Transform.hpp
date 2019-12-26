#pragma once

#include "Component.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Engine {

class Transform : public Component {
private:
    void calcDirectionVectors();
public:

    Transform();

    void translate(float x, float y, float z);
    void rotate(float x, float y, float z);
    void scale(float x, float y, float z);

    void setPosition(float x, float y, float z);
    void setRotation(float x, float y, float z);
    void setRotation(glm::quat _rotation);

	void setScale(float x, float y, float z);

    inline glm::vec3 getPosition() const {return position;}
    inline glm::quat getRotation() const {return rotation;}
    inline glm::vec3 getScale() const {return size;}
    inline glm::vec3 getForward() const {return forward;}
    inline glm::vec3 getRight() const {return right;}
    inline glm::vec3 getUp() const {return up;}

    glm::vec3 calcGlobalPosition();
    glm::quat calcGlobalRotation();
    glm::vec3 calcGlobalScale();

    glm::mat4 calcModelMatrix();
private:
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 size;
    glm::vec3 forward;
    glm::vec3 right;
    glm::vec3 up;
};

}
