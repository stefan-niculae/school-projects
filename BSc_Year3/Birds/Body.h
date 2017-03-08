#pragma once


class Body
{
public:
    std::vector<Transform*> transforms;

    void SetColor(glm::vec4 color)
    {
        for (auto transf : transforms)
            transf->SetColor(color);
    }

    void SetColor(std::string name)
    {
        for (auto transf : transforms)
            transf->SetColor(name);
    }

    void SetTranslation(glm::vec3 transl)
    {
        for (auto transf : transforms)
            transf->transl = transl;
    }
    void SetTranslation(float x, float y)
    {
        SetTranslation(glm::vec3(x, y, 0));
    }

    void SetScale(glm::vec3 scale)
    {
        for (auto transf : transforms)
            transf->scale = scale;
    }

    void SetRotation(float rot)
    {
        for (auto transf : transforms)
            transf->rot = rot;
    }
};

