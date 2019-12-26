#version 330

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in ivec3 jointIndices;
layout(location = 3) in vec3 jointWeights;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

uniform mat4 boneTransforms[100]; 

out vec3 vColor;
out vec3 fragNormal;

void main(){
    vec4 totalPosition = vec4(0.0);
    vec4 totalNormal = vec4(0.0); 
    
    for(int i = 0; i<3; i++){
        mat4 boneTransform = boneTransforms[jointIndices[i]];
        vec4 posePosition = boneTransform * vec4(vertexPosition_modelspace,1.0);
        totalPosition += posePosition * jointWeights[i];

        vec4 worldNormal = boneTransform * vec4(vertexNormal,0);
        totalNormal += worldNormal * jointWeights[i];
    }
    

    gl_Position = P*V*M * vec4(totalPosition.xyz, 1.0);
    fragNormal = normalize(V*M * vec4(totalNormal.xyz,0.0)).xyz;
    vColor = vec3(1.0);
}
