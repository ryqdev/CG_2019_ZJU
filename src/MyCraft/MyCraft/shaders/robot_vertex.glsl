#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
 
out vec3 ocolor;
 
 
uniform mat4 model;
uniform mat4 view;
uniform mat4 project;


void main()
{
    gl_Position =  project * view * model * vec4(aPos, 1.0);
  
       ocolor = aColor;
     
}