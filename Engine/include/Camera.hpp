#pragma once

#include <glm/glm.hpp>

#include "Component.hpp"

namespace Engine {

class Camera : public Component {
private:
    enum lookAtMode {
        DISABLED,
        POINT,
        OBJECT
    };
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
    glm::vec3 viewDirection;
    glm::vec3 targetPoint;
    GameObject* targetObject;
    lookAtMode lookAtState;
    float FOV;

    float width, height;
public:
    Camera(float _width, float _height);

    void setFOV(float fov);
    void updateAspect(float _width, float _height);

    void lookAt(GameObject* object);
    void lookAt(float x, float y, float z);
    void disableLookAt();

    virtual void start();
    virtual void lateUpdate();

    inline glm::vec3 getViewDirection() const {return viewDirection;};
    inline glm::mat4 getProjectionMatrix() const {return projectionMatrix;};
    inline glm::mat4 getViewMatrix() const {return viewMatrix;};
    inline float getWidth() const {return width;};
    inline float getHeight() const {return height;};
private:
    void calculateMatrices();
};

}
