#pragma once

#include "skybox.h"

//Массив моделей, полученный из main.h
extern CModel *MODELS[M_LEN];

//Объект сцены
class CEntity
{
public:
	//Конструктор объекта, получающий вид модели
	explicit CEntity(int m);
	virtual ~CEntity();

	//Отрисовка объекта с использованием определенной шейдерной программы, относительно определенной точки обзора
	virtual void Render(CShaderProgram* sp, glm::vec3* viewPos);
	//Метод-геттер для модели
	CModel* GetModel();

	//Обновить объект
	virtual void Update(glm::vec3 pos);
	//Выбрать новую текстуру для модели по имени файла
	virtual bool SetTexture(const char *);
	//Выбрать новую текстуру из загруженных по хэндлеру
	virtual bool SetTexture(GLuint tex);

	//Установить количесто сторон модели
	virtual void SetSides(GLuint sides_) { sides = sides_; }
	//Установить количество кадров 2Д анимации
	virtual void SetAnimated(GLuint animated_) { animated = animated_; }
	//Запустить 2Д анимацию
	virtual void PlayAnimation() { playing_anim = true; }

	//Переместить модель в определенную точку в мировых координатах
	void SetPosition(glm::vec3 pos_) { position = pos_; }
	//Метод-геттер для мировых координат объекта
	glm::vec3 GetPosition() { return position; }
	//Задать ориентацию объекта в пространстве углами Эйлера
	void SetOrientation(glm::vec3 angles_) { orientation = glm::quat(angles_); }
	//Задать масштаб объекта
	void SetScale(glm::vec3 scale_) { scale = scale_; }
	bool IsMovable() { return movable; }
	void MakeMovable() { movable = true; }
	bool IsHud() { return hud; }
	void MakeHud() { hud = true; }

protected:
	//Масштаб объекта
	glm::vec3 scale = glm::vec3(1.0f);
	//Позиция объекта в мировых координатах 
	glm::vec3 position = glm::vec3(0.0f);
	//Кватернион, описывающий ориентацию объекта в пространстве
	glm::quat orientation = glm::quat(glm::vec3(0.0f)); // => (0, 0, -1) направление по умолчанию
	//Модель, используемая обхектом
	CModel* model = nullptr;
	//Текстура, используемая объектом
	CTexture2D* texture = nullptr;
	//2D ANIMATION
	//Количество сторон объекта
	GLuint sides = 1;
	//Количество кадров 2Д анимации
	GLuint animated = 1;
	//Текущий кадр 2Д анимации
	GLuint cadr = 0;
	//Запущена ли 2Д анимация
	bool playing_anim = false;
	bool movable = false;
	bool hud = false;
};