#version 460 core

//in vec3 v_Position;
in vec2 v_TexCoords;
in vec3 v_Normal;
//in vec3 v_Color;

out vec4 fragColor;

uniform vec3 u_Light; 
uniform sampler2D u_Texture; 

void main()
{
	vec3 l = normalize(-u_Light);
	vec4 color = texture(u_Texture, v_TexCoords); 
	vec4 ambient = color * 0.1; 
	float cosTheta = max(dot(l, v_Normal), 0); 
	fragColor = color * cosTheta + ambient;
}
