#pragma once

#include <vector>
#include <list>
#include <stdlib.h>
#include <iostream>
#include <GLFW/glfw3.h>

namespace Engine {

class WindowEventSubscriber;
void windowResizeCallback(GLFWwindow* window, int width, int height);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void cursorCallback(GLFWwindow* window, double xpos, double ypos);
void mouseClickCallback(GLFWwindow* window, int button, int action, int mod);
void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

class WindowEventManager{
public:
    enum events {
        WINDOW_RESIZE = 0,
        KEYBOARD_INPUT = 1,
        CURSOR_POSITION = 2,
        MOUSE_BUTTON = 3,
        MOUSE_SCROLL = 4
    };
    static void setWindow(GLFWwindow* _window);
    static void subscribe(events eventList, WindowEventSubscriber* subscriber);
    static void unsubscribe(events eventList, WindowEventSubscriber* subscriber);

    friend void windowResizeCallback(GLFWwindow* window, int width, int height);
    friend void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    friend void cursorCallback(GLFWwindow* window, double xpos, double ypos);
    friend void mouseClickCallback(GLFWwindow* window, int button, int action, int mod);
    friend void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

private:
    static GLFWwindow* window;
    static std::vector<std::list<WindowEventSubscriber*> > subscribers;
    WindowEventManager();
};

class WindowEventSubscriber{
public:
    WindowEventSubscriber(){};
    void subscribe(WindowEventManager::events eventList);
    void unSubscribe(WindowEventManager::events eventList);

    friend void windowResizeCallback(GLFWwindow* window, int width, int height);
    friend void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    friend void cursorCallback(GLFWwindow* window, double xpos, double ypos);
    friend void mouseClickCallback(GLFWwindow* window, int button, int action, int mod);
    friend void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

protected:
    virtual void onWindowResize(int width, int height){return;};
    virtual void onKey(int key, int scancode, int action, int mods){return;};
    virtual void onCursorMove(double xpos, double ypos){return;};
    virtual void onMouseClick(int button, int action, int mod){return;};
    virtual void onMouseScroll(double xoffset, double yoffset){return;};
};

}
