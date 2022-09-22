#include <chrono>
#include "Time.h"

float Time::realtimeSinceStartup ;
float Time::deltaTime ;

void Time::Update() {
    static auto startTime = std::chrono::high_resolution_clock::now();
    static auto lastFrameTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();

    realtimeSinceStartup = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
    deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - lastFrameTime).count();
    lastFrameTime =currentTime;
}
