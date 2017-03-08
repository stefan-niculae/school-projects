#pragma once

#include "glm/glm.hpp"


const float PI = 3.14159265;

glm::vec4 ColorFromName(std::string name)
{
    return
    name == "black" ? glm::vec4(0, 0, 0, 1) :
    name == "white" ? glm::vec4(1, 1, 1, 1) :
    name == "red"   ? glm::vec4(1, 0, 0, 1) :
    name == "green" ? glm::vec4(0, 1, 0, 1) :
    name == "blue"  ? glm::vec4(0, 0, 1, 1) :
    glm::vec4(0, 0, 0, 1) ;

}

glm::vec3 operator* (float s, const glm::vec3 V)
{
    return glm::vec3(s * V.x, s * V.y, s * V.z);
}

float RadToDeg(float radians)
{
    return radians * 180 / PI;
}

// Random generating

float rand01()
{
    // TODO uniform dist from <random>
    static bool alreadySeeded = false;
    if (!alreadySeeded) {
        srand ((unsigned int)time(NULL));
        alreadySeeded = true;
    }
    return (float)rand() / RAND_MAX;
}

bool randBool()
{
    return rand01() > .5;
}

int randRange(int min, int max)
{
    return min + (rand() % (int)(max - min + 1));
}

float randRangef(float min, float max)
{
    return min + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(max - min)));
}

glm::vec3 randVec3(float min, float max)
{
    return glm::vec3(randRangef(min, max), randRangef(min, max), randRangef(min, max));
}
