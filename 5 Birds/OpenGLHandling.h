#pragma once

#include "LoadShaders.h"
#include "Bird.h"


auto birds = std::vector<Bird*>();
auto stars = std::vector<Transform*>();

int SCREEN_WIDTH  = 1600;
int SCREEN_HEIGHT = 800;


const int MAX_VERTEX_COUNT = 1 << 15;
GLfloat Coords[MAX_VERTEX_COUNT];
GLfloat Colors[MAX_VERTEX_COUNT];

GLuint
VaoId,
VboId,
ColorBufferId,
ProgramId;


void LoadVertices()
{
    // Create VBO
    
    // Create a new buffer
    glGenBuffers(1, &VboId);
    // Set it as the current buffer
    glBindBuffer(GL_ARRAY_BUFFER, VboId);
    // Copy vertices in current buffers
    glBufferData(GL_ARRAY_BUFFER, sizeof(Coords), Coords, GL_STATIC_DRAW);

    // Create, bind a VAO (Vertex Array Object)
    // Useful when more VBOs are used
    glGenVertexArrays(1, &VaoId);
    glBindVertexArray(VaoId);
    // Attribute 0 is posiiton
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

    // New buffer for the colors
    glGenBuffers(1, &ColorBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, ColorBufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
    // Attribute 1 is color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
}

void UnloadVertices()
{
    // Destroy VBO


    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &ColorBufferId);
    glDeleteBuffers(1, &VboId);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VaoId);
}

void Cleanup()
{
    // Destroy shaders
    glDeleteProgram(ProgramId);
    UnloadVertices();
}

void InitializeWindow(int argc, char* argv[], const char* windowName)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_3_2_CORE_PROFILE);

    glutInitWindowPosition(600, 25);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutCreateWindow(windowName);


    glewExperimental = GL_TRUE; // Apple compatibility
    glewInit();
    glClearColor (.4, .6, .9, 1);
}

void CreateShaders()
{
    ProgramId = LoadShaders("Transformations.vert", "Identity.frag");
    glUseProgram(ProgramId);
}

void SetMatrixg(const glm::mat4 matrix, std::string name)
{
    auto location = glGetUniformLocation(ProgramId, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
}

void RenderTransformg(Transform transf)
{
    static const auto RESIZE_MATRIX = glm::scale(I4, glm::vec3(1.f / SCREEN_WIDTH, 1.f / SCREEN_HEIGHT, 1));

    SetMatrixg(RESIZE_MATRIX, "resize");
    SetMatrixg(transf.Transformation(), "transformation");

    glDrawArrays(transf.mode, transf.loadIndex, transf.loadCount);
}

void SetMatrix(const char* name, const float matrix[4][4])
{
    GLuint location = glGetUniformLocation(ProgramId, name);
    glUniformMatrix4fv(location, 1, GL_TRUE, &matrix[0][0]);
}

void SetResize()
{
    float x = 1.f / SCREEN_WIDTH;
    float y = 1.f / SCREEN_HEIGHT;

    float matrix[4][4] =
    {
        { x,  0,  0,  0 },
        { 0,  y,  0,  0 },
        { 0,  0,  0,  0 },
        { 0,  0,  0,  1 },
    };

    SetMatrix("resize", matrix);
}

void SetScale(float x = 1, float y = 1)
{
    float matrix[4][4] =
    {
        { x,  0,  0,  0 },
        { 0,  y,  0,  0 },
        { 0,  0,  1,  0 },
        { 0,  0,  0,  1 },
    };

    SetMatrix("scale", matrix);
}

void SetScale(glm::vec3 V)
{
    SetScale(V.x, V.y);
}

void SetTranslation(float x = 0, float y = 0)
{
    float matrix[4][4] =
    {
        { 1,  0,  0,  x },
        { 0,  1,  0,  y },
        { 0,  0,  1,  0 },
        { 0,  0,  0,  1 },
    };

    SetMatrix("transl", matrix);
}

void SetTranslation(glm::vec3 V)
{
    SetTranslation(V.x, V.y);
}

inline float DegToRad(float d)
{
    return d / 180 * PI;
}

void SetRotation(float angle = 0)
{
    float rad = DegToRad(angle); // radians

    float matrix[4][4] =
    {
        { cosf(rad),   -sinf(rad),    0,      0 },
        { sinf(rad),    cosf(rad),    0,      0 },
        {         0,            0,    0,      0 },
        {         0,            0,    0,      1 },
    };

    SetMatrix("rot", matrix);
}

void SetOriginReturn(float currX, float currY)
{
    float nX = -currX;
    float nY = -currY;

    float nullifier[4][4] =
    {
        { 1, 0, 0, nX },
        { 0, 1, 0, nY },
        { 0, 0, 1, 0  },
        { 0, 0, 0, 1  }
    };
    SetMatrix("nullifier", nullifier);


    float rX = 2 * currX;
    float rY = 2 * currY;

    float restorer[4][4] =
    {
        { 1, 0, 0, rX },
        { 0, 1, 0, rY },
        { 0, 0, 1, 0  },
        { 0, 0, 0, 1  }
    };
    SetMatrix("restorer", restorer);
}

void SetOriginReturn(glm::vec3 currPos)
{
    SetOriginReturn(currPos.x, currPos.y);
}

void RenderTransform(Transform transform)
{
    SetResize();
    SetOriginReturn(transform.transl);

    SetTranslation(transform.transl);
    SetScale(transform.scale);
    SetRotation(transform.rot);

    // GL_POLYGON deprecated in OpenGL 3
    glDrawArrays(transform.mode, transform.loadIndex, transform.loadCount);
}

void Render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    LoadVertices();

    for (auto star : stars)
        RenderTransform(*star);

    // TODO change from bird to body here (to generalize)
    for (auto body : birds)
        for (auto transf : body->transforms)
            RenderTransform(*transf);

    glFlush();
    UnloadVertices();
}

void SetHandlers(void display(), void mouse(int, int, int, int), void idle(), void close())
{
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutIdleFunc(idle);
    glutWMCloseFunc(close);
}
