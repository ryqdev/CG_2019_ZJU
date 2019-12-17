#version 330 core
#extension GL_EXT_texture_array : enable
in vec3 texCoord;
out vec4 fragColor;

uniform sampler2DArray tex;

void main()
{
    fragColor = texture2DArray(tex, texCoord);
}