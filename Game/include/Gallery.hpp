#pragma once

#include "ShaderProgram.hpp"
#include "Component.hpp"
#include "Room.hpp"
#include "Hall.hpp"

class Gallery : public Engine::Component {
public:
    Gallery(Engine::ShaderProgram* program, std::string roomPath, float roomWidth, float roomLength, float roomHeight, std::string hallPath, float hallWidth, float hallLength, float hallHeight, Engine::GameObject* _player);

    virtual void start();
    virtual void update();
private:
    //point to private vectors
    Engine::GameObject* currentRoom;
    Engine::GameObject* currentHall;
    Engine::GameObject* nextRoom;
    
    Engine::GameObject* player;

    bool inHall;
    
private:
    //maps current hall in currentRoom space to new room space
    static constexpr int hallMap[] = { 2, 3, 0, 1};
    static constexpr int hallX[] = { 1, 0, -1, 0};
    static constexpr int hallZ[] = { 0, 1, 0, -1};
    static constexpr bool facesZ[] = {false, true, false, true};

    //generates the needed halls around a room
    void genRooms(Engine::GameObject* room);
    void genHalls(Engine::GameObject* room);

    Engine::GameObject* makeRoom(float x, float y, float z, int type);
    Engine::GameObject* makeHall(float x, float y, float z, bool facesZ, int type);
    
    std::vector<Engine::GameObject*> currentRooms;

    std::vector<Engine::GameObject*> currentHalls;
    std::vector<Engine::GameObject*> nextHalls;
};
