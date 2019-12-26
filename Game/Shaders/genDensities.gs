#version 330
layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

in int instanceID[];

flat out int layerIndex;

void main(){
    gl_Position = gl_in[0].gl_Position;
    gl_Layer = instanceID[0];
    layerIndex = instanceID[0];
    EmitVertex();
    gl_Position = gl_in[1].gl_Position;
    gl_Layer = instanceID[1];
    layerIndex = instanceID[1];
    EmitVertex();
    gl_Position = gl_in[2].gl_Position;
    gl_Layer = instanceID[2];
    layerIndex = instanceID[2];
    EmitVertex();
    EndPrimitive();
}
