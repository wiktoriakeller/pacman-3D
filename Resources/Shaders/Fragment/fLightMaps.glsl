#version 330 core
#define NR_POINT_LIGHTS 1

struct Material {
	bool useSpecularMap;
	bool useNormalMap;

	sampler2D diffuseMap1;
	sampler2D specularMap1;
	sampler2D normalMap1;
	float shininess;
};

struct DirectionalLight {
	vec3 direction;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	//attentuation variables
	float constant;
	float linear;
	float quadratic;
};

in vec3 fragmentPosition;
in vec2 textureCoord;
in mat3 TBN;
in vec3 surfaceNormal;

uniform Material uMaterial;
uniform DirectionalLight uDirLight;
uniform PointLight uPointLights[NR_POINT_LIGHTS];
uniform vec3 uViewPosition;

out vec4 fragmentColor;

//functions
vec3 CalculateDirLight(vec3 viewDir, vec3 normal);
vec3 CalculatePointLight(PointLight light, vec3 viewDir, vec3 normal);

void main() {
	//calculating normal vector
	vec3 normal = vec3(0.0, 0.0, 0.0);

	if(uMaterial.useNormalMap) {
		normal = texture(uMaterial.normalMap1, textureCoord).rgb;
		normal = normal * 2.0 - 1.0; //transforming from [0, 1] to [-1, 1]
		normal = normalize(TBN * normal);
	}
	else {
		normal = surfaceNormal;
	}

    vec3 result = vec3(0.0, 0.0, 0.0);
	vec3 viewDir = normalize(uViewPosition - fragmentPosition);

	result += CalculateDirLight(viewDir, normal);

	for(int i = 0; i < NR_POINT_LIGHTS; i++) {
		result += CalculatePointLight(uPointLights[i], viewDir, normal);
	}

	fragmentColor = vec4(result, 1.0);
}

vec3 CalculateDirLight(vec3 viewDir, vec3 normal) {
    vec3 lightDir = normalize(-uDirLight.direction);

	vec3 ambient = vec3(0.0, 0.0, 0.0);
	vec3 diffuse = vec3(0.0, 0.0, 0.0);
	vec3 specular = vec3(0.0, 0.0, 0.0);

	//ambient component
	ambient = uDirLight.ambient * texture(uMaterial.diffuseMap1, textureCoord).rgb;

	//diffuse component
	float diffuseStrength = max(dot(normal, lightDir), 0.0);
	diffuse = uDirLight.diffuse * diffuseStrength * texture(uMaterial.diffuseMap1, textureCoord).rgb;
    
	//specular component
	if (uMaterial.useSpecularMap) {
		vec3 reflectedLight = reflect(-lightDir, normal);
		float specularStrength = pow(max(dot(viewDir, reflectedLight), 0.0), uMaterial.shininess);
		specular = uDirLight.specular * specularStrength * texture(uMaterial.specularMap1, textureCoord).rgb;
	}

	return (ambient + diffuse + specular);
}

vec3 CalculatePointLight(PointLight light, vec3 viewDir, vec3 normal) {
    vec3 lightDir = normalize(light.position - fragmentPosition);

	vec3 ambient = vec3(0.0, 0.0, 0.0);
	vec3 diffuse = vec3(0.0, 0.0, 0.0);
	vec3 specular = vec3(0.0, 0.0, 0.0);

	//ambient component
	ambient = light.ambient * texture(uMaterial.diffuseMap1, textureCoord).rgb;

	//diffuse component
	float diffuseStrength = max(dot(normal, lightDir), 0.0);
	diffuse = light.diffuse * diffuseStrength * texture(uMaterial.diffuseMap1, textureCoord).rgb;
    
	//specular component
	if (uMaterial.useSpecularMap) {
		vec3 reflectedLight = reflect(-lightDir, normal);
		float specularStrength = pow(max(dot(viewDir, reflectedLight), 0.0), uMaterial.shininess);
		specular = light.specular * specularStrength * texture(uMaterial.specularMap1, textureCoord).rgb;
	}

	//calculating attentuation
	float dist = length(light.position - fragmentPosition);
	float attentuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

	ambient *= attentuation;
	diffuse *= attentuation;
	specular *= attentuation;

	return (ambient + diffuse + specular);
}