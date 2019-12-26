#pragma once


#include "Component.hpp"
#include "WindowEventManager.hpp"
#include "GameObject.hpp"
#include "Transform.hpp"
#include "Camera.hpp"
#include <glm/glm.hpp>

using namespace glm;
using namespace Engine;

class FirstPersonController : public Component, public WindowEventSubscriber{
private:
    float lastTime;

    float speed;
    float mouseSpeed;

    vec2 rotate;
    vec3 move;

    vec3 translationVector;
    vec2 rotationVector;
public:
    FirstPersonController();

    virtual void start();
    virtual void update();

private:
    virtual void onKey(int key, int scancode, int action, int mods);
    virtual void onCursorMove(double xpos, double ypos);

    void calcTranslation(float dt);
    void calcRotation(float dt);
};
