#pragma once

#include "GameObject.hpp"
#include "Window.hpp"

namespace Engine {

class Game {
private:
    void mainloop();
    void init();
    void cleanup();

    Window* win;
protected:
    virtual void start(){return;};
    virtual void update(){return;};
    virtual void finish(){return;}

    GameObject* root;
    GameObject* camera;
public:
    Game(){}
    void run();
};

}
