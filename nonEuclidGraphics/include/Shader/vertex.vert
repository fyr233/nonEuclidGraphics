#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition;
uniform vec3 cameraPos;
uniform vec3 cameraX;
uniform vec3 cameraY;
uniform vec3 cameraZ;
uniform mat3 G;
uniform mat4 P;
uniform mat4 M;

void main(){
    
    vec3 a = (M * vec4(vertexPosition, 1)).xyz - cameraPos;
    float px = dot(a, G * cameraX);
    float py = dot(a, G * cameraY);
    float pz = dot(a, G * cameraZ);
    
    vec4 view_pos = vec4(px, py, pz, 1.0f);
    gl_Position = P * view_pos;
}
