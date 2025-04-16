// #version 430
#version 410

#define MAX_LIGHTS 8

layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec3 vertNormal;
out vec3 varyingNormal;
// out vec3 varyingLightDir;
out vec3 varyingLightDir[MAX_LIGHTS]; //
out vec3 varyingVertPos;
// out vec3 varyingHalfVector;
out vec3 varyingHalfVector[MAX_LIGHTS]; //

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
uniform int numLights;					   //
uniform PositionalLight light[MAX_LIGHTS]; //
uniform Material material;
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform mat4 norm_matrix;

void main(void)
{
	varyingVertPos = (mv_matrix * vec4(vertPos, 1.0)).xyz;
	// varyingLightDir = light.position - varyingVertPos;
	varyingNormal = (norm_matrix * vec4(vertNormal, 1.0)).xyz;

	// varyingHalfVector =
	// 	normalize(normalize(varyingLightDir) + normalize(-varyingVertPos)).xyz;
	/*
	 */
	for (int i = 0; (i < MAX_LIGHTS) && (i < numLights); i++)
		varyingLightDir[i] = light[i].position - varyingVertPos, varyingHalfVector[i] = normalize(normalize(varyingLightDir[i]) + normalize(-varyingVertPos));

	gl_Position = proj_matrix * mv_matrix * vec4(vertPos, 1.0);
}
