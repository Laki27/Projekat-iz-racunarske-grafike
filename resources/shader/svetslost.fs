#version 330 core
out vec4 FragColor;

uniform vec3 boja;

void main(){
 FragColor = vec4(boja, 1.0f);
}