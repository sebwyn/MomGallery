#include "Gallery.hpp"

#include "Transform.hpp"
#include "Hall.hpp"
#include "OBJLoader.hpp"

#include <algorithm>

constexpr int Gallery::hallMap[];
constexpr int Gallery::hallX[];
constexpr int Gallery::hallZ[];
constexpr bool Gallery::facesZ[];

Gallery::Gallery(Engine::ShaderProgram* program, std::string roomPath, float roomWidth, float roomLength, float roomHeight, std::string hallPath, float hallWidth, float hallLength, float hallHeight, Engine::GameObject* _player)
: Engine::Component("Gallery"), player(_player) {
    Room::mesh = new Engine::VAO();
    Engine::OBJLoader::loadOBJ(Room::mesh, roomPath.c_str());
    Room::program = program; 
    Room::width = roomWidth;
    Room::height = roomHeight;
    Room::length = roomLength;
    Hall::mesh = new Engine::VAO();
    Engine::OBJLoader::loadOBJ(Hall::mesh, hallPath.c_str());
    Hall::program = program;
    Hall::width = hallWidth;
    Hall::height = hallHeight;
    Hall::length = hallLength;
}

void Gallery::start(){
    currentRoom = makeRoom(0, 0, 0, -1);
    inHall = false;
    currentHall = NULL;
    nextRoom = NULL;
    
    //generate all the halls around the lobby
    //handed is negative so all rooms are generated
    genHalls(currentRoom);
    
    currentHalls.assign(0, NULL);
    currentHalls.reserve(nextHalls.size());
    for(auto hall : nextHalls){
        currentHalls.push_back(hall);
    }
    nextHalls.assign(0, NULL);
    genRooms(currentRoom);
}

void Gallery::update(){
    Room* cRoom = (Room*)currentRoom->getComponent("room");
    //std::cout << inHall << std::endl;
    if(inHall){
        Room* nRoom = (Room*)nextRoom->getComponent("room");
        if(cRoom->doesCollide(player)){
            //returning to room
            for(auto hall : nextHalls){
                if(hall != currentHall){
                    delete hall;
                }    
            }
            nextHalls.assign(0, NULL);

            inHall = false;
            currentHall = NULL;  
            nextRoom = NULL;
        } else if(nRoom->doesCollide(player)){
            //going to a completely new room
            //we need to delete the rooms loaded around the old current room
            //and load the rooms aound the new current room
            for(auto room : currentRooms){
                if(room != nextRoom && room != currentRoom){
                    delete room;
                }
            }
            currentRooms.assign(0, NULL);
            Hall* cHall = (Hall*)currentHall->getComponent("hall");
            cRoom->type = hallMap[cHall->type];
            genRooms(nextRoom); 
            
            //going to new room
            for(auto hall : currentHalls){
                if(hall != currentHall){
                    delete hall;
                } 
            }
            currentHalls.assign(0, NULL);
            currentHalls.reserve(nextHalls.size());
            for(auto hall : nextHalls){
                currentHalls.push_back(hall);
            }
            nextHalls.assign(0, NULL);
            //map the hall that was passed to the new current room
            cHall->type = hallMap[cHall->type];
            
            currentRoom = nextRoom;
            inHall = false;
            currentHall = NULL; 
            nextRoom = NULL;
        }
        return;
    } else {
        for(auto hallObj : currentHalls){
            Hall* hall = (Hall*)hallObj->getComponent("hall");
            if(hall->doesCollide(player)){
                inHall = true;
                currentHall = hallObj;
                
                //move nextRoom to point to the correct next room
                for(auto roomObj : currentRooms){
                    Room* r = (Room*)roomObj->getComponent("room");
                    std::cout << r->type << " " << hall->type << std::endl;
                    if(r->type == hall->type){
                        nextRoom = roomObj;
                        break;
                    }
                }
                
                //might want to clear here
                std::cout << nextRoom << std::endl;
                genHalls(nextRoom); 
                break;
            }
        }   
    }    
}

//currentRoom and currentHall should still be set to last values
void Gallery::genRooms(Engine::GameObject* room){
    Engine::Transform* rTrans = (Engine::Transform*)room->getComponent("transform");
    glm::vec3 roomPos = rTrans->calcGlobalPosition(); 
    Room* r = (Room*)room->getComponent("room");
    Hall* lh = NULL;
    if(currentHall != NULL){
        lh = (Hall*)currentHall->getComponent("hall");
    }
    for(int i = 0; i < 4; i++){
        if(lh){
            if(i != hallMap[lh->type]){
                float rx=roomPos.x+hallX[i]*(Room::width+Hall::length);
                float ry=roomPos.y;
                float rz=roomPos.z+hallZ[i]*(Room::width+Hall::length);
                
                makeRoom(rx, ry, rz, i); 
            } else {
                currentRooms.push_back(currentRoom);
            }
        } else {
            float rx=roomPos.x+hallX[i]*(Room::width+Hall::length);
            float ry=roomPos.y;
            float rz=roomPos.z+hallZ[i]*(Room::width+Hall::length);
                
            makeRoom(rx, ry, rz, i); 
        }
    }    
}

//called when entering a hall
//generates the halls that need to be generate around the room the player is 
//entering
//handed is the type of the room local to the next room
void Gallery::genHalls(Engine::GameObject* room){
    Engine::Transform* roomTrans=(Engine::Transform*)room->getComponent("transform");
    glm::vec3 roomPos = roomTrans->calcGlobalPosition();
    Room* r = (Room*)room->getComponent("room");
    Hall* lh = NULL;
    if(currentHall != NULL){
        lh = (Hall*)currentHall->getComponent("hall"); 
    }
    for(int i = 0; i < 4; i++){
        if(lh){
            if(i != hallMap[lh->type]){
                int hx = roomPos.x + hallX[i] * (Room::width/2 + Hall::length/2); 
                int hy = roomPos.y;
                int hz = roomPos.z + hallZ[i] * (Room::width/2 + Hall::length/2);
                
                makeHall(hx, hy, hz, facesZ[i], i);
            } else {
                nextHalls.push_back(currentHall);
            }
        } else {
            int hx = roomPos.x + hallX[i] * (Room::width/2 + Hall::length/2); 
            int hy = roomPos.y;
            int hz = roomPos.z + hallZ[i] * (Room::width/2 + Hall::length/2);
            std::cout << hx << " " << hz << std::endl;    
            makeHall(hx, hy, hz, facesZ[i], i);
        }
    }
}

Engine::GameObject* Gallery::makeRoom(float x, float y, float z, int type){
    Engine::GameObject* roomObj = new Engine::GameObject("room", true);
    Room* room = new Room(x, y, z, type);
    roomObj->attachComponent(room);
    
    currentRooms.push_back(roomObj);
    object->addChild(roomObj);

    return roomObj;
}

Engine::GameObject* Gallery::makeHall(float x, float y, float z, bool facesZ, int type){
    Engine::GameObject* hallObj = new Engine::GameObject("hall", true);
    Hall* hall = new Hall(x, y, z, facesZ, type);
    hallObj->attachComponent(hall);

    nextHalls.push_back(hallObj);
    object->addChild(hallObj);

    return hallObj;
}

