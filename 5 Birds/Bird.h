#pragma once

#include "Body.h"
#include "Utils.h"


class Bird : public Body
{
public:

    Transform leftWing;
    Transform rightWing;
    Transform torso;

    Bird(glm::vec3 _start, glm::vec3 _dest)
    {
        BuildBody();

        start = _start;
        dest = _dest;

        RandomizeStats();

        GoToStart();
    }

    void BuildBody()
    {
        const int FX = 280;
        const int FY = -15;
        const int TX = 140;
        const int TY = 40;


        const int Y  = 30;
        const int X  = 22;
        const int WX = 40;
        const int WY = 10;

        rightWing = Transform(
                              {
                                  Vertex( FX, FY),
                                  Vertex( TX, TY),
                                  Vertex( WX, WY),
                                  Vertex( WX, FY),
                              });
        leftWing = Transform(
                             {
                                 Vertex(-FX, FY),
                                 Vertex(-TX, TY),
                                 Vertex(-WX, WY),
                                 Vertex(-WX, FY),
                             });
        rightWing.SetRotationPoint( WX, 0);
        leftWing .SetRotationPoint(-WX, 0);

        torso = Transform(
                          {
                              Vertex(-WX, -WY), // bottom left link
                              Vertex(-WX,  WY), // top left link

                              Vertex(-X ,  Y ), // top left corner
                              Vertex( X ,  Y ), // top right corner

                              Vertex( WX,  WY), // top right link
                              Vertex( WX, -WY), // bottom right link

                              Vertex( X , -Y ), // bottom right corner
                              Vertex(-X , -Y ), // bottom left corner
                          });
        
        transforms = { &leftWing, &torso, &rightWing };
    }

    void RandomizeStats()
    {
        SetScale(randVec3(.25, .8));
        SetColor(glm::vec4(randVec3(0, .35), 1));
        initialWingRot = randRangef(-15, 15);

        duration = randRangef(3, 8.5);
        wingDir = randBool() ? 1 : -1;

        start += randVec3(-150, 150);
    }


    glm::vec3 start;
    glm::vec3 dest;

    void GoToStart()
    {
        SetTranslation(start);
        RotateToDestination();

        t = 0;
        momentum = 1;

        // Ignore path rotation when flapping wings
        directedTopBound = WING_TOP_BOUND + torso.rot;
        directedLowBound = WING_LOW_BOUND + torso.rot;
    }

    float initialWingRot;

    void RotateToDestination()
    {
        // Bring the whole path such that the start is the origin
        float x = dest.x - start.x;
        float y = dest.y - start.y;
        float rad = atan2(y, x);
        float deg = RadToDeg(rad);
        deg = deg - 90;

        SetRotation(deg);

        // FIXME
//        leftWing.rot  =  initialWingRot;
//        rightWing.rot = -initialWingRot;
    }

    const float MOVE_SPEED = .03;
    float momentum;

    void Move()
    {
        // Stop when reaching the destination
        if (t < duration) {
            SetTranslation(
                           Interpolate(start.x, dest.x, t / duration),
                           Interpolate(start.y, dest.y, t / duration));
            t += MOVE_SPEED * momentum;
        }

        FlapWings();
    }

    float duration;
    float t;

    float Interpolate(float from, float to, float time)
    {
        return (1 - time) * from + time * to;
    }


    const float WING_SPEED = 4;
    int wingDir;

    const float WING_TOP_BOUND = -30;
    const float WING_LOW_BOUND = 25;

    float directedTopBound;
    float directedLowBound;

    void FlapWings()
    {
        if (leftWing.rot <= directedTopBound) {
            wingDir = +1;
            momentum = 2; // go faster when pushing down;
        }
        if (leftWing.rot >= directedLowBound) {
            wingDir = -1;
            momentum = 1;
        }

        float velocity = WING_SPEED * wingDir * momentum;
        leftWing.rot  += velocity;
        rightWing.rot -= velocity;
    }

};
