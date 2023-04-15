#include "Material.h"

Material::Material() {
	speculaIntensity = 0.0f;
	shininess = 0.0f;
}

Material::Material(GLfloat sIntensity, GLfloat shine) {
	speculaIntensity = sIntensity;
	shininess = shine;
}

void Material::UseMaterial(GLuint specularIntensityLocation, GLuint shinessLocation) 
{
	glUniform1f(specularIntensityLocation, speculaIntensity);
	glUniform1f(shinessLocation, shininess);
}

Material::~Material() 
{

}
