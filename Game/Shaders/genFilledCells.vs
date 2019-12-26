#version 330

uniform int chunkSize;
uniform float isoLevel = 0.0;

uniform sampler3D densities;

out uint x8_y8_z8_case8;

void main(){
    int y = gl_VertexID / chunkSize;
    int x = gl_VertexID - (y * chunkSize); //my own mod function
    int z = gl_InstanceID;
    int xp1 = x + 1;
    int yp1 = y + 1;
    int zp1 = z + 1;

    float v1 = texelFetch(densities, ivec3(x  , y  , z  ), 0).r;
    float v2 = texelFetch(densities, ivec3(xp1, y  , z  ), 0).r;
    float v3 = texelFetch(densities, ivec3(xp1, y  , zp1), 0).r;
    float v4 = texelFetch(densities, ivec3(x  , y  , zp1), 0).r;
    float v5 = texelFetch(densities, ivec3(x  , yp1, z  ), 0).r;
    float v6 = texelFetch(densities, ivec3(xp1, yp1, z  ), 0).r;
    float v7 = texelFetch(densities, ivec3(xp1, yp1, zp1), 0).r;
    float v8 = texelFetch(densities, ivec3(x  , yp1, zp1), 0).r;

    uint cubeCase = uint(0);
    if(v1 < isoLevel) cubeCase |= uint(1);
    if(v2 < isoLevel) cubeCase |= uint(2);
    if(v3 < isoLevel) cubeCase |= uint(4);
    if(v4 < isoLevel) cubeCase |= uint(8);
    if(v5 < isoLevel) cubeCase |= uint(16);
    if(v6 < isoLevel) cubeCase |= uint(32);
    if(v7 < isoLevel) cubeCase |= uint(64);
    if(v8 < isoLevel) cubeCase |= uint(128);
    
    x8_y8_z8_case8 = uint(x << 24) | uint(y << 16) | uint(gl_InstanceID << 8) | cubeCase;
}
