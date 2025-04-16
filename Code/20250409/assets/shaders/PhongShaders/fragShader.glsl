// #version 430
#version 410

#define MAX_LIGHTS 8 //

in vec3 varyingNormal;
// in vec3 varyingLightDir;
in vec3 varyingLightDir[MAX_LIGHTS]; //
in vec3 varyingVertPos;

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
uniform int numLights;					   //
uniform PositionalLight light[MAX_LIGHTS]; //
uniform Material material;
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform mat4 norm_matrix;

void main(void)
{
	// normalize the light, normal, and view vectors:
	// vec3 L = normalize(varyingLightDir);
	vec3 N = normalize(varyingNormal);
	vec3 V = normalize(-varyingVertPos);

	// // compute light reflection vector, with respect N:
	// vec3 R = normalize(reflect(-L, N));

	// // get the angle between the light and surface normal:
	// float cosTheta = dot(L, N);

	// // angle between the view vector and reflected light:
	// float cosPhi = dot(V, R);

	// compute ADS contributions (per pixel):
	// vec3 ambient = ((globalAmbient * material.ambient) + (light.ambient * material.ambient)).xyz;
	// vec3 diffuse = light.diffuse.xyz * material.diffuse.xyz * max(cosTheta, 0.0);
	// vec3 specular = light.specular.xyz * material.specular.xyz * pow(max(cosPhi, 0.0), material.shininess);
	vec3 ambient = (material.ambient * globalAmbient).xyz, diffuse = vec3(0.0), specular = vec3(0.0); //

	/*
	 */
	for (int i = 0; (i < MAX_LIGHTS) && (i < numLights); ++i)
	{
		vec3 L = normalize(varyingLightDir[i]), R = reflect(-L, N);

		float cosTheta = dot(N, L);

		ambient += (material.ambient * light[i].ambient).xyz;

		if (cosTheta > 0.0)
		{
			diffuse += material.diffuse.xyz * light[i].diffuse.xyz * cosTheta;
			float cosPhi = dot(R, V);
			if (cosPhi > 0.0)
				specular += material.specular.xyz * light[i].specular.xyz * pow(cosPhi, material.shininess);
		}
	}
	if (numLights > 0)
		diffuse /= float(numLights), specular /= float(numLights);

	fragColor = vec4((ambient + diffuse + specular), 1.0);
}
