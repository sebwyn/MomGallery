#version 330 core

in vec3 fragNormal;

uniform vec3 c;
in vec3 vColor;

out vec3 color;

void main(){
    vec3 lightColor = vec3(1,1,1);
    vec3 lightDirection = normalize(vec3(2,1,0));
    float diffuseLighting = 1;

    vec3 albedo = vec3(0.05,.05,0.1);
    float ambientLighting = 0.1;

    float gamma = 2.2;

    vec3 diffuse = clamp(dot(fragNormal,lightDirection),0,1) * diffuseLighting * lightColor;
    vec3 ambient = albedo*ambientLighting;
    color = (diffuse+ambient) * c * vColor;
    color.rgb = pow(color.rgb, vec3(1.0/gamma));
}

