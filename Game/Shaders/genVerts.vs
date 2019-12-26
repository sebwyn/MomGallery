#version 330

uint cornerIndexAFromEdge[12] = uint[12](
    uint(0),
    uint(1),
    uint(2),
    uint(3),
    uint(4),
    uint(5),
    uint(6),
    uint(7),
    uint(0),
    uint(1),
    uint(2),
    uint(3)
);

uint cornerIndexBFromEdge[12] = uint[12](
    uint(1),
    uint(2),
    uint(3),
    uint(0),
    uint(5),
    uint(6),
    uint(7),
    uint(4),
    uint(4),
    uint(5),
    uint(6),
    uint(7)
);

ivec3 cornerPosFromIndex[8] = ivec3[8](
    ivec3(0, 0, 0),
    ivec3(0, 0, 1),
    ivec3(0, 1, 0),
    ivec3(0, 1, 1),
    ivec3(1, 0, 0),
    ivec3(1, 0, 1),
    ivec3(1, 1, 0),
    ivec3(1, 1, 1)
);

layout(location = 0) in uint x8_y8_z8_null4_edge4;

out Vertex {
    vec3 iPosition;
    vec3 iNormal;
}; 

uniform float isoLevel = 0.0;
//chunk size plus 1
uniform int chunkSize;

//densities with GL_LINEAR sampling
uniform sampler3D densities;

void main(){
    //calculate the position and normal
    //calculate position
    //this position should still be in localized range [0, chunkSize]
    ivec3 start = ivec3((x8_y8_z8_null4_edge4 >> 24) & uint(0xFF),
                        (x8_y8_z8_null4_edge4 >> 16) & uint(0xFF),
                        (x8_y8_z8_null4_edge4 >>  8) & uint(0xFF));
    uint edge = x8_y8_z8_null4_edge4 & uint(0x0F);
    ivec3 v1 = start + cornerPosFromIndex[cornerIndexAFromEdge[edge]];
    ivec3 v2 = start + cornerPosFromIndex[cornerIndexBFromEdge[edge]];
    float v1D = texelFetch(densities, v1, 0).r;
    float v2D = texelFetch(densities, v2, 0).r; 
    float t = (isoLevel - v1D) / (v2D - v1D);
    
    //generate the final position in [0, 1]
    //generate the position in this range, because each chunk will be 1 by 1
    iPosition = mix(vec3(v1)/vec3(chunkSize), vec3(v2)/vec3(chunkSize), t);
    
    float d = 1.0/chunkSize;
    vec3 grad;
    grad.x = texture(densities, iPosition + vec3(d, 0, 0)).r - 
             texture(densities, iPosition - vec3(d, 0, 0)).r;
    grad.y = texture(densities, iPosition + vec3(0, d, 0)).r - 
             texture(densities, iPosition - vec3(0, d, 0)).r;
    grad.z = texture(densities, iPosition + vec3(0, 0, d)).r - 
             texture(densities, iPosition - vec3(0, 0, d)).r;

    iNormal = -1*normalize(grad);
}
