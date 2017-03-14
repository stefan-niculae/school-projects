//#version 400
//
//
//in vec4 in_Position;
//in vec4 in_Color;
//
//out vec4 ex_Color;
//
//uniform mat4 resize;
//uniform mat4 transformation;
//
//
//void main()
//{
//	gl_Position = resize * transformation * in_Position;
//	ex_Color = in_Color;
//}

#version 400


in vec4 in_Position;
in vec4 in_Color;

out vec4 ex_Color;

uniform mat4 resize;
uniform mat4 scale;
uniform mat4 transl;
uniform mat4 rot;

uniform mat4 nullifier;
uniform mat4 restorer;


void main()
{
    // Apply translation, rotation, scaling in this order
    gl_Position = (resize * restorer * scale * rot * nullifier * transl) * in_Position;
    ex_Color = in_Color;
}
