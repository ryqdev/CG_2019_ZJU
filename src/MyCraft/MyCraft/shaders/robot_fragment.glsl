#version 330 core
#extension GL_EXT_texture_array : enable
 
out vec4 fragColor;
in vec3 ocolor;
 
void main()
{
  
       fragColor=vec4(ocolor,1.0);
     
    
}