#include "WindowEventManager.hpp"

namespace Engine {

GLFWwindow* WindowEventManager::window;
std::vector<std::list<WindowEventSubscriber*> > WindowEventManager::subscribers;

void WindowEventManager::setWindow(GLFWwindow* _window){
    window = _window;
    for (int i = 0; i < 5; i++){
        subscribers.push_back(std::list<WindowEventSubscriber*>());
    }

    glfwSetWindowSizeCallback(window, windowResizeCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, cursorCallback);
    glfwSetMouseButtonCallback(window, mouseClickCallback);
    glfwSetScrollCallback(window, mouseScrollCallback);
}

void WindowEventManager::subscribe(events eventList, WindowEventSubscriber* subscriber){
    subscribers[eventList].push_back(subscriber);
}

void WindowEventManager::unsubscribe(events eventList, WindowEventSubscriber* subscriber){
    subscribers[eventList].remove(subscriber);
}

void windowResizeCallback(GLFWwindow* window, int width, int height){
    std::list<WindowEventSubscriber*> subscriberList = WindowEventManager::subscribers[WindowEventManager::WINDOW_RESIZE];
    std::list<WindowEventSubscriber*>::iterator i;
    for (i = subscriberList.begin(); i != subscriberList.end(); ++i){
        (*i)->onWindowResize(width, height);
    }
}

void keyCallback(GLFWwindow* window,int key, int scancode, int action, int mods){
    std::list<WindowEventSubscriber*> subscriberList = WindowEventManager::subscribers[WindowEventManager::KEYBOARD_INPUT];
    std::list<WindowEventSubscriber*>::iterator i;
    for (i = subscriberList.begin(); i != subscriberList.end(); ++i){
        (*i)->onKey(key, scancode, action, mods);
    }
}

void cursorCallback(GLFWwindow* window, double xpos, double ypos){
    std::list<WindowEventSubscriber*> subscriberList = WindowEventManager::subscribers[WindowEventManager::CURSOR_POSITION];
    std::list<WindowEventSubscriber*>::iterator i;
    for (i = subscriberList.begin(); i != subscriberList.end(); ++i){
        (*i)->onCursorMove(xpos, ypos);
    }
}

void mouseClickCallback(GLFWwindow* window, int button, int action, int mod){
    std::list<WindowEventSubscriber*> subscriberList = WindowEventManager::subscribers[WindowEventManager::MOUSE_BUTTON];
    std::list<WindowEventSubscriber*>::iterator i;
    for (i = subscriberList.begin(); i != subscriberList.end(); ++i){
        (*i)->onMouseClick(button, action, mod);
    }
}

void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset){
    std::list<WindowEventSubscriber*> subscriberList = WindowEventManager::subscribers[WindowEventManager::MOUSE_SCROLL];
    std::list<WindowEventSubscriber*>::iterator i;
    for (i = subscriberList.begin(); i != subscriberList.end(); ++i){
        (*i)->onMouseScroll(xoffset, yoffset);
    }
}

void WindowEventSubscriber::subscribe(WindowEventManager::events eventList){
    WindowEventManager::subscribe(eventList, this);
}

void WindowEventSubscriber::unSubscribe(WindowEventManager::events eventList){
    WindowEventManager::unsubscribe(eventList, this);
}

}
