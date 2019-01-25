#pragma once
#include <glad/glad.h>
#include "shaders.h"
#include "glmaths.h"
#include "sides.h"

enum LIGHT_TYPES {L_DIRECTIONAL = 0, L_POINT, L_SPOTLIGHT, L_LEN};

//extern List<CShaderProgram*> SHADERS;

class CLight
{
public:
	CLight() {}

	void SetPosition(glm::vec3 pos) { position = pos; }
	void SetColor(glm::vec3 col) { color = col; }
	virtual void prepareShadowMap() {};
	LIGHT_TYPES type;
	glm::vec3 GetPos() { return position; }
	GLuint GetWidth() { return width; }
	GLuint GetHeight() { return height; }
	GLuint GetDepthMap() { return depthMap; }
	GLuint GetDepthMapFBO() { return depthMapFBO; }
	void SetDepthMapFBO(GLuint i) { depthMapFBO = i; }
	void SetDepthMap(GLuint i) { depthMap = i; }

protected:
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 color;
	GLuint width = 1024, height = 1024;

	GLuint depthMapFBO, depthMap;

};

class CLightDirectional : public CLight
{
public:
	CLightDirectional() = default;
	CLightDirectional(glm::vec3 dir, glm::vec3 pos);
	void prepareShadowMap();

};

class CLightSpot : CLight
{
public:
	CLightSpot();

protected:
	
};

class CLightPoint : CLight
{
public:
	CLightPoint();

protected:
	
};