#pragma once

#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "WindowEventManager.hpp"

namespace Engine {

class Window : public WindowEventSubscriber {
private:
    GLFWwindow* window;
    int width, height;
    bool cursorEnabled;
    bool shouldClose;
public:
    Window(const char* title, int width, int height);
    void update() const;
    void clear() const;
    void disableCursor();
    void enableCursor();
    inline bool closed() const {return ((glfwWindowShouldClose(window) != 0)||shouldClose);};
    inline int getWidth() const {return width;};
    inline int getHeight() const {return height;};
    inline GLFWwindow* getWindow() const {return window;};
    ~Window(){};

    virtual void onWindowResize(int width, int height);
    virtual void onKey(int key, int scancode, int action, int mods);
};

}
