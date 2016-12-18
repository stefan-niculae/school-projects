#pragma once

#include <string>
#include <iostream>
#include <fstream>

#include <GL/glew.h>


std::string Content(std::string path)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Couldn't open " << path << std::endl;
        throw 1;
    }

    std::string content;
    std::string line;
    while(getline(file, line))
        content += line + '\n';

    file.close();
    return content;
}

void CompileShader (std::string path, GLuint id, std::string code, bool logActions)
{
    if (logActions)
        std::cout << "Compiling shader " << path << std::endl;

    char const* sourcePointer = code.c_str();
    glShaderSource (id, 1, &sourcePointer, NULL);
    glCompileShader (id);
}

void Check (std::string path, GLuint id)
{
    GLint status;
    glGetProgramiv(id, GL_LINK_STATUS, &status);
    if (!status)
        return;

    int logLength;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);

    if (logLength > 0)
    {
        char error[logLength];
        int length;
        glGetShaderInfoLog(id, logLength, &length, &error[0]);

        if (length > 0)
            std::cerr << "Error in " << path << ":" << std::endl << error << std::endl;
    }
}

GLuint LoadShaders (const char* vertPath, const char* fragPath, bool logActions = false)
{
    std::string vertCode, fragCode;
    try {
        vertCode = Content(vertPath);
        fragCode = Content(fragPath);
    } catch (int err) {
        return 0;
    }


    GLuint vertId = glCreateShader (GL_VERTEX_SHADER);
    GLuint fragId = glCreateShader (GL_FRAGMENT_SHADER);


    CompileShader (vertPath, vertId, vertCode, logActions);
    Check (vertPath, vertId);

    CompileShader(fragPath, fragId, fragCode, logActions);
    Check(fragPath, fragId);


    if (logActions)
        std::cout << "Linking program" << std::endl;
    GLuint progId = glCreateProgram();
    glAttachShader(progId, vertId);
    glAttachShader(progId, fragId);
    glLinkProgram(progId);

    Check("linking program", progId);


    glDetachShader(progId, vertId);
    glDetachShader(progId, fragId);

    glDeleteShader(vertId);
    glDeleteShader(fragId);
    
    return progId;
}