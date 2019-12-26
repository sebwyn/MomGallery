#version 330

uniform sampler3D tex;
uniform float progression;

in vec2 uv;

out vec3 color;

void main(){
    color = vec3(texture(tex, vec3(uv, progression)).r);
}
