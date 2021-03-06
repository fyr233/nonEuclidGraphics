#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexTexCoord;
layout(location = 2) in vec3 vertexNormal;

uniform mat4 V;
uniform mat4 P;
uniform mat4 M;

uniform mat3 SR;

out vec2 TexCoord;
out vec3 SRnormal;
out vec3 WorldPos;

void main(){
    
    vec3 world_pos = (M * vec4(vertexPosition, 1.0f)).xyz;
    gl_Position = P * V * vec4(world_pos, 1.0f);
    TexCoord = vertexTexCoord;
    SRnormal = SR * vertexNormal;
    WorldPos = world_pos;
}
