#include "Window.hpp"

using namespace glm;
using namespace Engine;

Window::Window(const char* title, int _width, int _height) : WindowEventSubscriber(){
    width = _width;
    height = _height;
    shouldClose = false;

    glewExperimental = true;
    if (!glfwInit()){
        fprintf( stderr, "Failed to initialize GLFW\n");
        return;
    }
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow( width, height, title, NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);

    glfwSetWindowUserPointer(window, this);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
    }
    WindowEventManager::setWindow(window);
    WindowEventSubscriber::subscribe(WindowEventManager::events::WINDOW_RESIZE);
    WindowEventSubscriber::subscribe(WindowEventManager::events::KEYBOARD_INPUT);
}

void Window::update() const {
    glfwWaitEventsTimeout(0.016);
    //glViewport(0, 0, width, height);
    glfwSwapBuffers(window);
    if (!cursorEnabled) glfwSetCursorPos(window, width/2, height/2);
}

void Window::clear() const {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::disableCursor(){
    cursorEnabled = false;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glfwRawMouseMotionSupported())
    glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
}

void Window::enableCursor(){
    cursorEnabled = true;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
}

void Window::onWindowResize(int width, int height){
    Window* win = (Window*)glfwGetWindowUserPointer(window);
    win->width = width;
    win->height = height;
}

void Window::onKey(int key, int scancode, int action, int mods){
    if (action == GLFW_PRESS){
        switch(key){
            case GLFW_KEY_Q:
                shouldClose = true;
                break;
            case GLFW_KEY_ESCAPE:
                if (cursorEnabled){
                    disableCursor();
                }else{
                    enableCursor();
                }
                break;
            default:
                break;
        }
    }
}
