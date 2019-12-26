#version 330
layout(points) in;
layout(points, max_vertices=1) out;

in Vertex {
    vec3 iPosition;
    vec3 iNormal;
} vertIn[];

out vec3 position;
out vec3 normal;

void main(){
    position = vertIn[0].iPosition; 
    normal = vertIn[0].iNormal; 
    EmitVertex();
    EndPrimitive();
}
