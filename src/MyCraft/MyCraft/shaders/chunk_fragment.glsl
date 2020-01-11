#version 330 core
#extension GL_EXT_texture_array : enable

in vec3 tangentFragPos;
in vec3 tangentLightPos;
in vec3 tangentViewPos;
in vec3 texCoord;
 
out vec4 fragColor;
 
struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform sampler2DArray tex;
uniform float shiness;
uniform vec3 lightDir;
uniform Light light;

vec3 calculateLighting(){
    // object
    vec3 objectColor = texture2DArray(tex, vec3(texCoord.xy, texCoord.z * 3)).rgb;
    vec3 objectSpecular = texture2DArray(tex, vec3(texCoord.xy, texCoord.z * 3 + 1)).rgb;
    vec3 normal = texture2DArray(tex, vec3(texCoord.xy, texCoord.z * 3 + 2)).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    
    // ambient
    vec3 ambient = objectColor * light.ambient;

    // diffuse 
    vec3 lightDir = normalize(tangentLightPos - tangentFragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = objectColor * diff * light.diffuse;

    // specular
    vec3 viewDir = normalize(tangentViewPos - tangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shiness);
    vec3 specular = objectSpecular * spec * light.specular;
 
    return (ambient + diffuse + specular);
}

void main()
{
    fragColor = vec4(calculateLighting(), 1.0);
}