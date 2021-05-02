#include "Light.h"

Light::Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) :
	ambient(ambient),
	diffuse(diffuse),
	specular(specular) { }

Light::~Light() { }

glm::vec3 Light::GetAmbient() const {
	return ambient;
}

glm::vec3 Light::GetDiffuse() const {
	return diffuse;
}

glm::vec3 Light::GetSpecular() const {
	return specular;
}