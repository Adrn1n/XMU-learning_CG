// #version 430
#version 410 //

layout(location = 0) in vec3 position;
out vec3 tc;

uniform mat4 v_matrix;
uniform mat4 proj_matrix;
// layout(binding = 0) uniform samplerCube samp;
uniform samplerCube samp; //

void main(void)
{
    tc = position;
    mat4 v3_matrix = mat4(mat3(v_matrix));
    gl_Position = proj_matrix * v3_matrix * vec4(position, 1.0);
}
