#version 330 core
in vec2 TexCoord;

out vec4 color;

uniform float mix_value;

uniform sampler2D our_texture1;
uniform sampler2D our_texture2;

void main()
{
    color=mix(texture(our_texture1,TexCoord),texture(our_texture2,TexCoord),mix_value);
}
