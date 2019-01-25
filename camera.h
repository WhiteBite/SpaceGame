#pragma once
#include "glmaths.h"

class CCamera
{
public:
	//Матрица 4х4 проекции
	glm::mat4 m_projection;
	//Матрица 4х4 вида
	glm::mat4 m_view;

	CCamera();
	//Векторное произведение матрицы проекции и матрицы вида
	glm::mat4 Look();
	//Повернуть вектор направления обзора камеры
	void Rotate(float amount, const glm::vec3 &dir);
	//Переместить камеру в определенном направлении
	void Translate(const glm::vec3 &dir);
	//Обновить камеру
	void Update(float speed);
	//Методы-геттеры для направления обзора камеры, его проекции на плоскость
	// вектора направления вверх и текущей позиции камеры, соответственно
	glm::vec3 GetDir();
	glm::vec3 GetDir2D();
	glm::vec3 GetUp();
	glm::vec3 GetPos();
	//Установить позицию камеры, вектор ее направления обзора: в пространстве и относительно плоскости,
	// угла обзора, соответственно
	void SetPos(glm::vec3 pos);
	void SetDir(glm::vec3 dir);
	void SetDir2D(glm::vec3 dir);
	void SetFov(float fov_);
	//Метод-геттер угла обзора
	float GetFov();
	//MOVEMENT
	//Поворот камеры, с учетом сдвига курсора мыши
	void RotateDirection(float mouseX, float mouseY);
	//Переместить камеру
	void Move(float speed);
	//Сместить кмеру относительно правого вектора
	void Strafe(float speed);
	//Задать движение камеры, относительно вектора направления обзора
	void SetMoving(float m);
	//Задать движение камеры, относительно правого вектора
	void SetStrafing(float m);

private:
	//Желаемый угол обзора
	float fov;
	//Текущий угол обзора
	float current_fov;
	//Чувствительность мыши
	float sensetivity = 0.03f;
	//матрица направлений движения
	float moving[2] = {0, 0};
	//Позиция камеры в мировых координатах
	glm::vec3 position;
	//Вектор направления обзора
	glm::vec3 direction;
	//Вектор направления вверх
	glm::vec3 up;

};