#version 460 core

//in vec3 v_Position;
in vec2 v_TexCoords;
// in vec3 v_Normal;
//in vec3 v_Color;

out vec4 fragColor;

uniform sampler2D u_Texture; 

void main()
{
	fragColor = texture(u_Texture, v_TexCoords);
}
