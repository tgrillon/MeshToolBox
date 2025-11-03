#version 460 core

// layout(location = 0) in vec3 v_Position;
layout(location = 1) in vec2 v_TexCoords;
// layout(location = 2) in vec3 v_Normal;
// layout(location = 3) in vec3 v_Color;

out vec4 fragColor;

uniform sampler2D u_Texture; 

void main()
{
	fragColor = vec4(0, 0, 1, 1); //texture(u_Texture, v_TexCoords);
}
