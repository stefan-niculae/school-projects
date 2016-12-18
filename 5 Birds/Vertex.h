#pragma once

#include "glm/glm.hpp"


class Vertex
{
public:

    glm::vec4 coords;
    glm::vec4 color;

    Vertex (const glm::vec4 _coords = glm::vec4(0, 0, 0, 1),
            const glm::vec4 _color  = glm::vec4(0, 0, 0, 1))
    : coords(_coords), color(_color)
    { }

    Vertex (float x = 0, float y = 0, std::string colorName = "black")
    {
        coords = glm::vec4(x, y, 0, 1);
        color = ColorFromName(colorName);
    }

};
