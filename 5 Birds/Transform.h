#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" // scale, rotate, translate

static const auto I4 = glm::mat4(1);

class Transform
{
public:

    std::vector<Vertex> vertices;

    glm::vec3 transl;
    glm::vec3 scale;
    float rot;

    bool rotateAroundCenter;
    glm::vec3 hinge; // rotation point

    GLenum mode;
    int loadIndex;
    int loadCount;


    Transform(std::vector<Vertex> _vertices = std::vector<Vertex>(),
              glm::vec3 _transl        = glm::vec3(0, 0, 0),
              glm::vec3 _scale         = glm::vec3(1, 1, 1),
              float _rot               = 0,
              GLenum _mode             = GL_TRIANGLE_FAN)
    : vertices(_vertices), transl(_transl), scale(_scale), rot(_rot), mode(_mode)
    { }

    glm::mat4 Transformation() const
    {
        static const auto ROT_VECT = glm::vec3(0, 0, 1); // where does this come from??

        auto translation = glm::translate(I4, transl);
        auto scaling     = glm::scale(I4, scale);
        auto rotation    = glm::rotate(I4, rot, ROT_VECT);

        auto rotationPoint = rotateAroundCenter ? transl : hinge;

        auto nullifier   = glm::translate(I4, -rotationPoint);
        auto restorer    = glm::translate(I4, 2.f * rotationPoint);

        return restorer * rotation * scaling * nullifier * translation;
    }

    void SetColor(glm::vec4 color)
    {
        for (auto& vert : vertices)
            vert.color = color;
    }

    void SetColor(std::string name)
    {
        SetColor(ColorFromName(name));
    }

    Vertex operator[](int index) const
    {
        return vertices[index];
    }
    Vertex& operator[](int index)
    {
        return vertices[index];
    }

    void SetRotationPoint(float x, float y)
    {
        hinge = glm::vec3(x, y, 0);
        rotateAroundCenter = false;
    }
};
