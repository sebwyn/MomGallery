#version 330

const vec3 verts[6] = vec3[6](
    vec3(-1.0, -1.0, 0.0),
    vec3( 1.0, -1.0, 0.0),
    vec3( 1.0,  1.0, 0.0),

    vec3(-1.0, -1.0, 0.0),
    vec3( 1.0,  1.0, 0.0),
    vec3(-1.0,  1.0, 0.0)
);

const vec2 uvs[6] = vec2[6](
    vec2(0.0, 0.0),
    vec2(1.0, 0.0),
    vec2(1.0, 1.0),
    
    vec2(0.0, 0.0),
    vec2(1.0, 1.0),
    vec2(0.0, 1.0)
);

layout(location = 0) in vec2 pos;

out vec2 uv;

void main(){
    gl_Position = vec4(verts[gl_VertexID], 1.0);
    uv = uvs[gl_VertexID];
}
