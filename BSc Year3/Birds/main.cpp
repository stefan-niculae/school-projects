#include <iostream>
#include <vector>
#include <math.h> // acos

#include <GL/glew.h>
#include <GLUT/glut.h>
#include "glm/glm.hpp" // vec3, vec4

#include "Utils.h"
#include "Vertex.h"
#include "Transform.h"
#include "OpenGLHandling.h"
#include "Bird.h"


using namespace std;


// Vertex Management

int vertexCount;
int starVertexCount;

void ResetVertices()
{
    vertexCount = starVertexCount;
}

// For debugging purposes
void DisplayMatrix(std::string name)
{
    std::cout << name << ": " << std::endl;

    for (int i = 0; i < vertexCount * 4; i++)
    {
        if (i > 0 && i % 4 == 0)
            std::cout << std::endl;
        if (name == "coords")
            std::cout << Coords[i] << ' ';
        else if (name == "colors")
            std::cout << Colors[i] << ' ';
    }

    std::cout << std::endl << std::endl;
}


void AddVertex(Vertex V)
{
    int i = vertexCount * 4;

    Coords[i + 0] = V.coords.x;
    Coords[i + 1] = V.coords.y;
    Coords[i + 2] = V.coords.z;
    Coords[i + 3] = V.coords.w;

    Colors[i + 0] = V.color.x;
    Colors[i + 1] = V.color.y;
    Colors[i + 2] = V.color.z;
    Colors[i + 3] = V.color.w;

    vertexCount++;
}

void AddVertices(Transform* transf)
{
    transf->loadIndex = vertexCount;
    transf->loadCount = (int)transf->vertices.size();

    for (auto vert : transf->vertices)
        AddVertex(vert);
}

void AddVertices(Body* body)
{
    for (auto transf : body->transforms)
        AddVertices(transf);
}

void AddVertices(vector<Bird*> birds)
{
    for (auto body : birds)
        AddVertices(body);
}




// Bird Handling

void MoveBirds()
{
    for (auto bird : birds)
        bird->Move();

    glutPostRedisplay();
}

void SpawnBirds(glm::vec3 start, glm::vec3 dest)
{
    // Create a random amount of birds
    int birdCount = randRange(4, 6);
    for (int i = 0; i < birdCount; i++)
        birds.push_back(new Bird(start, dest));

    // Add their vertices for glut to handle them
    AddVertices(birds);
}


void MouseHandler(int button, int state, int x, int y)
{
    // Defined only for left button click
    if (button != GLUT_LEFT_BUTTON)
        return;

    // Make height be 0 on the bottom, not on the top
    y  = SCREEN_HEIGHT - y;
    // Make coordinates range from -height/2 to height/2 instead of 0 to height (same for width)
    x -= SCREEN_WIDTH  / 2;
    y -= SCREEN_HEIGHT / 2;

    static glm::vec3 start;

    if (state == GLUT_DOWN) {
        // Clear previous birds
        birds.clear();
        ResetVertices();

        // Register new start point
        start = glm::vec3(x, y, 0);
    }
    if (state == GLUT_UP)
        // Spawn the birds with the destination here
        SpawnBirds(start, glm::vec3(x, y, 0));
}

void CreateStars()
{
    const int count = randRange(10, 20);

    for (int i = 0; i < count; i++)
    {
        const float FY = 75;
        const float FX = 0;
        const float NY = 20;
        const float NX = 20;
        auto star = new Transform({
            // Top
            Vertex(-NX,  NY), // left
            Vertex( FX,  FY), // far
            Vertex( NX,  NY), // right

            // Right
            Vertex( NX,  NY), // top
            Vertex( FY,  FX), // far
            Vertex( NX, -NY), // bottom

            // Bottom
            Vertex( NX, -NY), // right
            Vertex( FX, -FY), // bottom
            Vertex(-NX, -NY), // left

            // Left
            Vertex(-NX, -NY), // bottom
            Vertex(-FY, -FX), // left
            Vertex(-NX,  NY), // top
        });
        stars.push_back(star);

        star->scale = randRangef(.4, .6) * glm::vec3(1);
        star->SetColor(glm::vec4(
                                 randRangef(.85, 1),
                                 randRangef(.85, 1),
                                 randRangef(  0, .15),
                                 1));

        star->transl.x = randRangef(-SCREEN_WIDTH / 2, SCREEN_WIDTH / 2);
        star->transl.y = randRangef(-SCREEN_HEIGHT / 2, SCREEN_HEIGHT / 2);

        AddVertices(star);

        starVertexCount += star->vertices.size();
    }
}


int main (int argc, char* argv[])
{
    InitializeWindow(argc, argv, "Stol de Pasari (Lilieci)");
    SetHandlers(Render, MouseHandler, MoveBirds, Cleanup);
    CreateShaders();

    CreateStars();

    glutMainLoop();
}
