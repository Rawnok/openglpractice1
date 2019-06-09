#shader vertex
#version 330 core

layout(location = 0) in vec4 position; // which index to pull in from, this is t he index stated above in the first parameter of glVertexAttribPointer function

void main(){
	gl_Position = position;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform vec4 u_Color;

void main(){
	color = u_Color; //rgba
};