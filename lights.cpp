#include "lights.h"

CLightDirectional::CLightDirectional(glm::vec3 dir, glm::vec3 pos)
{
	position = pos;
	direction = dir;
	
	////framebuffer
	
}

void CLightDirectional::prepareShadowMap()
{
	
}
