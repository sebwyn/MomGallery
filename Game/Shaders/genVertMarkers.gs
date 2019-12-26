#version 330
layout(points) in;
layout(points, max_vertices=3) out;

in uint x8_y8_z8_null5_edge3[];

out uint x8_y8_z8_null4_edge4;

void main(){

    uint x8_y8_z8_null8 = x8_y8_z8_null5_edge3[0] & uint(0xFFFFFF00);

    //if((x8_y8_z8_null5_edge3[0] & uint(1)) > uint(0)){
        x8_y8_z8_null4_edge4 = x8_y8_z8_null8 | uint(3);  
        //x8_y8_z8_null4_edge4 = uint(0xFFFFFFFF);
        EmitVertex();
        EndPrimitive();
    //}
    //if((x8_y8_z8_null5_edge3[0] & uint(2)) > uint(0)){
        x8_y8_z8_null4_edge4 = x8_y8_z8_null8 | uint(0);  
        //x8_y8_z8_null4_edge4 = uint(0xFFFFFFFF);
        EmitVertex(); 
        EndPrimitive();
    //}
    //if((x8_y8_z8_null5_edge3[0] & uint(4)) > uint(0)){
        x8_y8_z8_null4_edge4 = x8_y8_z8_null8 | uint(8);  
        //x8_y8_z8_null4_edge4 = uint(0xFFFFFFFF);
        EmitVertex(); 
        EndPrimitive();
    //}
}
