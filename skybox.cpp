#include "skybox.h"

CSkybox::CSkybox(const char *path)
{
	model = MODELS[M_SKYBOX];
	SetTexture(path);
}

bool CSkybox::SetTexture(const char *path)
{
	auto texture_ = new CTexture3D(path, 1, GL_LINEAR);
	if (!texture_->isLoaded) return false;
	this->texture0 = texture_;

	auto texture_1 = new CTexture3D(path + 1, 1, GL_LINEAR);
	if (!texture_->isLoaded) return false;
	this->texture1 = texture_1;

	auto texture_2 = new CTexture3D(path + 2, 1, GL_LINEAR);
	if (!texture_->isLoaded) return false;
	this->texture2 = texture_2;

	return true;
}

void CSkybox::Render(CShaderProgram* sp, CCamera* camera)
{
	position = camera->GetPos();
	if (!camera || !texture0)
		return;
	else 
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture0->gl_texID);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture1->gl_texID);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture2->gl_texID);
		model->Render(sp, &camera->GetPos(), &position, &orientation, &scale);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
}