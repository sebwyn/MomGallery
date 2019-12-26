#version 330
layout(points) in;
layout(points, max_vertices=1) out;

in int layer[];
in int vertID[];

flat out uint vertexID;

void main(){
    gl_Position = gl_in[0].gl_Position;
    gl_Layer = layer[0];
    vertexID = uint(vertID[0]);
    EmitVertex();
}
