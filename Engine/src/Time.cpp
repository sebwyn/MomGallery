#include "Time.hpp"

using namespace Engine;

std::chrono::time_point<std::chrono::high_resolution_clock> Time::startPoint;
std::chrono::time_point<std::chrono::high_resolution_clock> Time::lastUpdate;
double Time::deltaUpdate;
double Time::timeSinceStart;

void Time::start(){
    startPoint = std::chrono::high_resolution_clock::now(); 
    lastUpdate = startPoint;
    deltaUpdate = 0.0;
}

void Time::update(){
    auto now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = now - lastUpdate;
    std::chrono::duration<double> elapsedStart = now - startPoint;
    deltaUpdate = elapsed.count();
    timeSinceStart = elapsedStart.count();
    
    lastUpdate = now;
}
