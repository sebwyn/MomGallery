#version 330

layout(location = 0) in uint x8_y8_z8_case8;

out uint x8_y8_z8_null5_edge3;

void main(){
    uint cube_case = x8_y8_z8_case8 & uint(0xFF);
    int  bit0 = int(cube_case     ) & 1;
    int  bit3 = int(cube_case >> 3) & 1;
    int  bit1 = int(cube_case >> 1) & 1;
    int  bit4 = int(cube_case >> 4) & 1;
    ivec3 build_vert_on_edge = abs( ivec3(bit3,bit1,bit4) - ivec3(bit0, bit0, bit0));

    uint bits = x8_y8_z8_case8 & uint(0xffffff00);
    if (build_vert_on_edge.x != 0)
        bits |= uint(1);
    if (build_vert_on_edge.y != 0)
        bits |= uint(2);
    if (build_vert_on_edge.z != 0)
        bits |= uint(4);

    x8_y8_z8_null5_edge3 = bits;
}
