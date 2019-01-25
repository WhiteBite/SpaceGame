#pragma once

#include "shaders.h"
#include "camera.h"
#include "lights.h"
#include "texture.h"

//Перечисление видов моделей
enum MODEL_TYPES {
	M_SMART_SPRITE = 0, M_CUBE, M_BILLBOARD, M_SKYBOX, M_FULLSCREEN, M_PLANE, M_CUSTOM_MODEL, M_LEN
};

//Класс, описывающий 3Д модель
class CModel 
{
public:
	//Конструктор, принимающий название файла модели в качестве аргумента, а также ее тип
	explicit CModel(std::string path, int rt = M_CUSTOM_MODEL);
	~CModel() {};

	//Метод отрисовки модели
	void Render(CShaderProgram* sp, glm::vec3* viewPos, glm::vec3* pos, glm::quat* orientation, glm::vec3* scale);
	//Метод смены режима отрисовки модели
	void SetRenderMode(GLuint mode) { RENDER_MODE = mode; }

	//Список вершин модели
	std::vector < glm::vec3 > vertecies;
	//Список текстурных координат для uv-развертки модели
	std::vector < glm::vec2 > uvs;
	//Список векторов нормалей модели
	std::vector < glm::vec3 > normals;
	//Список цветов вершин модели
	std::vector < glm::vec3 > colors;
	//Индексы вершин модели для режима отрисовки по индексам (в данной версии не используется)
	std::vector < GLint > indicies;

protected:

	//Хэндлер объекта массива вершин на видеокарте
	GLuint uiVAO;
	//Хэндлер объекта буффера вершин на видеокарте
	GLuint uiVBO;
	//Хэндлер объекта буффера индексов вершин на видеокарте
	GLuint uiEBO;
	//Количество вершин модели
	GLuint nodes;
	//Вид модели
	GLuint render_type = M_BILLBOARD;
	//Режим отрисовки модели (сетка/модель)
	GLenum RENDER_MODE = GL_TRIANGLES;

};

//Загрузчик моделей
class CModelLoader
{
public:
	//Метод загрузки моделей
	bool LoadModel(CModel* m, std::string path);
};