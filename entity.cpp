#include "entity.h"

CEntity::CEntity(int m)
{
	model = MODELS[m];
}

CEntity::~CEntity()
{
	
}

void CEntity::Update(glm::vec3 pos)
{
	if (!movable)
		return;
	glm::vec3 dir = pos - position;
	dir = glm::normalize(dir);
	dir *= 0.15f;
	position += dir;
}

CModel* CEntity::GetModel()
{
	return model;
}

bool CEntity::SetTexture(const char *name) {
	auto texture_ = new CTexture2D(name, sides, animated);
	if (!texture_->isLoaded) return false;
	this->texture = texture_;
	return true;
}

bool CEntity::SetTexture(GLuint tex) 
{
	this->texture->gl_texID = tex;
	return true;
}

void CEntity::Render(CShaderProgram* sp, glm::vec3* viewPos)
{
	if (model)
	{
		if (texture)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture->gl_texID);
			GLint side = GetSide(position - *viewPos, orientation * glm::vec3(0.0f, 0.0f, -1.0f));
			int cadre = side * animated + cadr / 15;

			if (playing_anim)
			{
				cadr++;
				cadr %= animated * 60;
			}
			else
				cadr = 0;
			sp->SetInt(cadre, "cadre");
			sp->SetInt(animated, "atlasX");
			sp->SetInt(sides, "atlasY");
		}
		model->Render(sp, viewPos, &position, &orientation, &scale);

		glBindVertexArray(0);
		sp->SetInt(0, "cadre");
		sp->SetInt(1, "atlasX");
		sp->SetInt(1, "atlasY");
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}