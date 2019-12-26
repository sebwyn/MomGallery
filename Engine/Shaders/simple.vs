#version 330

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec3 vertexColor;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec3 fragNormal;
out vec3 vColor;

void main(){
    gl_Position = P*V*M * vec4(vertexPosition_modelspace, 1.0);
    fragNormal = normalize(M * vec4(vertexNormal,0.0)).xyz;
    vColor = vec3(1.0);
}
