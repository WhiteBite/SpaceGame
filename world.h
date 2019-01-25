#pragma once
#include "entity.h"
#include "list.h"
#include "tree.h"

//Списки шейдерных программ, загруженные из main.h
extern CShaderProgram* GENERAL_SHADER;
extern CShaderProgram* SKYBOX_SHADER;
extern int killed_mobs;

//Сцена
class CWorld 
{
public:
	~CWorld();
	//Обновление и перерисовка сцены относительно определенной камеры, с учетом номера кадра
    bool UpdateWorld(CCamera* camera, GLint frame);
	//Обновление и перерисовка сцены относительно определенной камеры
	void RenderWorld(CCamera* camera);
	//Перерисовка сцены относительно определенной камеры
	void RenderSkybox(CCamera* camera);
	//Добавить новый объект в сцену
    void AddEntity(CEntity*);
	void DeleteEntity(CEntity*);
	//Изменить способ отрисовки обхектов сцены
	void SetRenderMode(GLenum mode);
	//Изменить размер окна
	void SetScreenSize(unsigned int w, unsigned int h);
	//Сменить скайбокс на следующий в списке
	void NextSkybox();
	//Сменить скайбокс на предыдущий в списке
	void PrevSkybox();
	//Выбрать шейдерную программу для отрисовки объектов
	void SetGeneralShader(int i) { general_shader = i; };
	//Выбрать шейдерную программу для отрисовки скайбокса
	void SetSkyboxShader(int i) { skybox_shader = i; };
	void Restart();
	//Указатель на используемый скайбокс
	CSkybox* skybox;

private:
	//Список объектов сцены
    std::vector<CEntity*> current_entities;
	//Дерево объектов сцены
	sTree entities_by_priority;
	//Номер кадра
	GLint frame = 0;
	unsigned int cycle = 0;
	//Номер используемого скайбокса
	int sky = 0;
	//Размеры экрана
	unsigned int SCR_WIDTH, SCR_HEIGHT;

	//Номера используемых шейдерных программ для обхектов и скайбокса, соответственно
	int general_shader = 0;
	int skybox_shader = 0;
};