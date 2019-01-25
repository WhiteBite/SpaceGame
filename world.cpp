#include "world.h"

CWorld::~CWorld()
{
	for (auto &e : current_entities)
		delete e;
}

bool CWorld::UpdateWorld(CCamera* camera, GLint frame)
{
	bool res = true;
	this->frame = frame;

	for (auto &e : current_entities)
	{
		if (e->IsHud())
			continue;
		e->Update(camera->GetPos());
		if (glm::length(camera->GetPos() - e->GetPosition()) < 1.7f)
			res = false;
		for (auto &ex : current_entities)
		{
			if (ex->IsHud())
				continue;
			if (ex != e)
				if (glm::length(ex->GetPosition() - e->GetPosition()) < 2.0f)
				{
					if (e->IsMovable())
						killed_mobs++;
					DeleteEntity(e);
					if (ex->IsMovable())
						killed_mobs++;
					DeleteEntity(ex);
				}
		}
	}

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	RenderSkybox(camera);
	RenderWorld(camera);
	return res;
}

void CWorld::RenderSkybox(CCamera* camera)
{
	SKYBOX_SHADER->UseProgram();
	SKYBOX_SHADER->SetMat4(&camera->m_projection[0][0], "P");
	SKYBOX_SHADER->SetMat4(&camera->m_view[0][0], "V");
	glm::vec3 tmp = camera->GetPos();
	SKYBOX_SHADER->SetVec3(&tmp[0], "viewPos");
	SKYBOX_SHADER->SetInt(frame, "frame");
	SKYBOX_SHADER->SetInt(0, "texSky0");
	SKYBOX_SHADER->SetInt(1, "texSky1");
	SKYBOX_SHADER->SetInt(2, "texSky2");
	glActiveTexture(GL_TEXTURE0);

	skybox->Render(SKYBOX_SHADER, camera);
}

void CWorld::RenderWorld(CCamera* camera)
{
	GENERAL_SHADER->UseProgram();

	GENERAL_SHADER->SetInt(0, "texSampler");
	GENERAL_SHADER->SetInt(1, "shadowMap");
	GENERAL_SHADER->SetInt(frame, "frame");

	GENERAL_SHADER->SetMat4(&camera->m_projection[0][0], "P");
	GENERAL_SHADER->SetMat4(&camera->m_view[0][0], "V");

	glm::vec3 tmp = camera->GetPos();
	GENERAL_SHADER->SetVec3(&tmp[0], "viewPos");

	glActiveTexture(GL_TEXTURE0);

	for (auto &e : current_entities)
		e->Render(GENERAL_SHADER, &camera->GetPos());
}

void CWorld::SetRenderMode(GLenum mode)
{
	for (auto e : current_entities)
		e->GetModel()->SetRenderMode(mode);
	skybox->GetModel()->SetRenderMode(mode);
}

void CWorld::AddEntity(CEntity* entity) {
	this->current_entities.push_back(entity);
}

void CWorld::DeleteEntity(CEntity* entity) {
	int i = 0;
	for (auto &e : current_entities)
	{
		if (e == entity)
			current_entities.erase(current_entities.begin() + i);
		i++;
	}
}

void CWorld::SetScreenSize(unsigned int w, unsigned int h)
{
	SCR_HEIGHT = h;
	SCR_WIDTH = w;
}

void CWorld::Restart()
{
	current_entities.clear();
}