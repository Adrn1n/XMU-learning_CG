// #version 430
#version 410 //

#define MAX_LIGHTS 8 //

in vec4 varyingColor;
out vec4 fragColor;

//  uniforms match those in the vertex shader,
//  but are not used directly in this fragment shader

struct PositionalLight
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec3 position;
};

struct Material
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};

uniform vec4 globalAmbient;
// uniform PositionalLight light;
uniform int numLights;					   //
uniform PositionalLight light[MAX_LIGHTS]; //
uniform Material material;
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform mat4 norm_matrix;

//  interpolate lighted color
// (interpolation of gl_Position is automatic)

void main(void)
{
	fragColor = varyingColor;
}
