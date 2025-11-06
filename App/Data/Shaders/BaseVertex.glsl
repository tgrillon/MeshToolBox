#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoords;
// layout(location = 2) in vec3 a_Normal;
// layout(location = 3) in vec3 a_Color;

//out vec3 v_Position;
out vec2 v_TexCoords;
// out vec3 v_Normal;
//out vec3 v_Color;

uniform mat4 u_Mvp; 

void main()
{
	gl_Position = u_Mvp * vec4(a_Position, 1.0);

	// v_Position = gl_Position.xyz; 
	v_TexCoords = a_TexCoords; 
	// v_Normal = a_Normal; 
	// v_Color = a_Color; 
}
