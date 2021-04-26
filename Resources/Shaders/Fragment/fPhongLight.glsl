#version 330 core

struct Material {
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;
	float shininess;
};

struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 normal;
in vec3 fragmentPosition;

uniform Material uMaterial;
uniform Light uLight;
uniform vec3 uViewPosition;

out vec4 fragmentColor;

void main() {
	//ambient component
	vec3 ambient = uLight.ambient * uMaterial.ambientColor;
	
	//diffuse component
	vec3 surfaceNormal = normal;
	vec3 lightDirection = normalize(uLight.position - fragmentPosition);
	float diffuseStrength = max(dot(surfaceNormal, lightDirection), 0.0);
	vec3 diffuse = uLight.diffuse * (diffuseStrength * uMaterial.diffuseColor);

	//specular component
	vec3 viewDirection = normalize(uViewPosition - fragmentPosition);
	vec3 reflectedLightDirection = reflect(-lightDirection, surfaceNormal);
	float specularStrength = pow(max(dot(viewDirection, reflectedLightDirection), 0.0), uMaterial.shininess);
	vec3 specular = uLight.specular * (specularStrength * uMaterial.specularColor);

	//end color
	vec3 result = ambient + diffuse + specular;
	fragmentColor = vec4(result, 1.0);
}