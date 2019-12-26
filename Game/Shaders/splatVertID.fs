#version 330

flat in uint vertexID; 

//write to framebuffer
layout(location = 0) out uint index;

void main(){
    index = uint(0xFFFFFFFF);
}
