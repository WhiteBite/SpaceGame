#pragma once
#include "model.h"

//Получение массива моделей из main.h для использования моделей в процессе отрисовки скайбокса
extern CModel *MODELS[M_LEN];

//Скайбокс
class CSkybox
{
public:
	CSkybox() {};
	//Конструктор, получающий название используемой текстуры
	CSkybox(const char *path);
	//Выбрать текстуру для заполнения скайбокса
	bool SetTexture(const char *path);
	//Отрисовать скайбокс, используя определенную шейдерную программу, относительно определенной камеры
	void Render(CShaderProgram* sp, CCamera* camera);
	//Функция-геттер для модели скайбокса
	CModel* GetModel() { return model; }

private:
	//Указатель на используемую модель
	CModel* model = nullptr;
	//Указатель на кубическую текстуру скайбокса
	CTexture3D* texture0 = nullptr;
	CTexture3D* texture1 = nullptr;
	CTexture3D* texture2 = nullptr;
	//Матрица аффинных преобразований
	glm::mat4 translation = glm::mat4(1.0f);
	//Смещение скайбокса по координатам
	glm::vec3 position = glm::vec3(0.0f);
	//Кватернион ориентации скайбокса
	glm::quat orientation = glm::quat(glm::vec3(0.0f));
	//Масштаб скайбокса 
	glm::vec3 scale = glm::vec3(1.0f);
};