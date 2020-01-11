#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTexCoord;
layout (location = 3) in vec3 aTangent;
 
out vec3 tangentFragPos;
out vec3 tangentLightPos;
out vec3 tangentViewPos;
out vec3 texCoord;
 
uniform mat4 model;
uniform mat4 view;
uniform mat4 project;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    gl_Position =  project * view * model * vec4(aPos, 1.0);
    texCoord = aTexCoord;
    
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 B = normalize(normalMatrix * cross(aTangent, aNormal));
    vec3 N = normalize(normalMatrix * aNormal);

    mat3 TBN = transpose(mat3(T, B, N));
    tangentFragPos = TBN * vec3(model * vec4(aPos, 1.0));
    tangentLightPos = TBN * lightPos;
    tangentViewPos = TBN * viewPos;
}