#version 330 core
#extension GL_EXT_texture_array : enable
in vec3 texCoord;
 
out vec4 fragColor;
 
uniform sampler2DArray tex;
in vec3 vNormal;
in vec3 FragPos;
uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shiness;
uniform vec3 lightcolor;
uniform vec3 lightpos;
uniform vec3 viewpos;

vec3 calculateLighting(){
     // ambient
    
    vec3 ambient = ambient * lightcolor ;

    // diffuse 
    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(lightpos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diffuse * lightcolor  ;

    // specular
 
    vec3 viewDir = normalize(viewpos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shiness);
 
    vec3 specular = specular* spec * lightcolor;
 
    return (diffuse + ambient + specular);
}

void main()
{
  
    
      fragColor = texture2DArray(tex, texCoord)* vec4(calculateLighting(),1.0);
   
}