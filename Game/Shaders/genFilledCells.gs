#version 330
layout(points) in;
layout(points, max_vertices=1) out;

in uint x8_y8_z8_case8[];

out uint oxyzc; 

void main(){
    uint c = x8_y8_z8_case8[0] & uint(0xFF);
    if((c != uint(0)) && (c != uint(255))){
        oxyzc = x8_y8_z8_case8[0];
        EmitVertex();
        EndPrimitive();
    }
}
