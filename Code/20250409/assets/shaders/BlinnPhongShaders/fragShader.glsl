// #version 430
#version 410

#define MAX_LIGHTS 16

in vec3 varyingNormal;
// in vec3 varyingLightDir;
in vec3 varyingLightDir[MAX_LIGHTS]; //
in vec3 varyingVertPos;
// in vec3 varyingHalfVector;
in vec3 varyingHalfVector[MAX_LIGHTS]; //

out vec4 fragColor;

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
uniform PositionalLight light[MAX_LIGHTS]; //
uniform int numLights;					   //
uniform Material material;
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform mat4 norm_matrix;

void main(void)
{
	// // normalize the light, normal, and view vectors:
	// vec3 L = normalize(varyingLightDir);
	vec3 N = normalize(varyingNormal);
	vec3 V = normalize(-varyingVertPos);

	// // get the angle between the light and surface normal:
	// float cosTheta = dot(L, N);

	// // halfway vector varyingHalfVector was computed in the vertex shader,
	// // and interpolated prior to reaching the fragment shader.
	// // It is copied into variable H here for convenience later.
	// vec3 H = normalize(varyingHalfVector);

	// // get angle between the normal and the halfway vector
	// float cosPhi = dot(H, N);

	// // compute ADS contributions (per pixel):
	// vec3 ambient = ((globalAmbient * material.ambient) + (light.ambient * material.ambient)).xyz;
	vec3 ambient = (globalAmbient * material.ambient).xyz;
	// vec3 diffuse = light.diffuse.xyz * material.diffuse.xyz * max(cosTheta, 0.0);
	// vec3 specular = light.specular.xyz * material.specular.xyz * pow(max(cosPhi, 0.0), material.shininess * 3.0);
	vec3 diffuse = vec3(0.0);
	vec3 specular = vec3(0.0);

	/*
	 */
	for (int i = 0; (i < MAX_LIGHTS) && (i < numLights); i++)
	{
		vec3 L = normalize(varyingLightDir[i]), H = normalize(varyingHalfVector[i]);
		float cosTheta = dot(L, N), cosPhi = dot(H, N);
		ambient += (light[i].ambient * material.ambient).xyz;
		if (cosTheta > 0.0)
		{
			diffuse += light[i].diffuse.xyz * material.diffuse.xyz * cosTheta;
			if (cosPhi > 0.0)
				specular += light[i].specular.xyz * material.specular.xyz * pow(cosPhi, material.shininess * 3.0);
		}
	}
	if (numLights > 0)
		diffuse /= float(numLights), specular /= float(numLights);

	fragColor = vec4((ambient + diffuse + specular), 1.0);
}
