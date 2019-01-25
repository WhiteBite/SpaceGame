#pragma once

#include <cstdlib>
#include <glad/glad.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

//Перечисление видов шейдерных программ в зависимости от типа объектов, к которым она применяется
enum SHADER_TYPES {SH_GENERAL = 0, SH_SKYBOX, SH_DIRECTIONAL_LIGHT, SH_SIMPLE, SH_LEN};

//Класс шейдера
class CShader
{
public:
	CShader();
	//Конструктор, получающий имя файла и тип шейдера
	CShader(const std::string &sFile, GLenum a_iType);
	~CShader();

	//Загрузка шейдера
	bool LoadShader(const std::string &sFile, GLenum a_iType);
	//Удаление шейдера и очистка памяти
	void DeleteShader();

	//Метод-геттер для проверки, загружен ли шейдер
	bool isLoaded();
	//Метод-геттер для получения хэндлера шейдера в памяти видеокарты
	GLuint GetShaderID();

private:
	//Хэндлер шейдера в памяти видеокарты
	GLuint shaderID;
	//Тип шейдера
	int type;
	//Загружен ли шейдер
	bool loaded;
};

//Шейдерная программа
class CShaderProgram
{
public:
	CShaderProgram();
	//Конструктор, получающий вершинный и фрагментный шейдеры
	CShaderProgram(CShader* v, CShader* f);
	~CShaderProgram();

	//Создать шейдерную программу
	void CreateProgram();
	//Удалить шейдерную программу и очистить память
	void DeleteProgram();

	//Прицепить новый шейдер к шейдерной программе
	bool AddShaderToProgram(CShader *shShader);
	//Скомпилировать шейдерную программу и загрузить ее в память видеокарты
	bool LinkProgram();

	//Использовать шейдерную программу
	void UseProgram();
	//Передать uniform-переменную в память видеокарты для испольщования ее шейдерной программой
	//Вектор из 3 float
	void SetVec3(float* begin, const char* var);
	//Матрица 4х4 float
	void SetMat4(float* begin, const char* var);
	//Одно значение int
	void SetInt(int begin, const char* var);
	//Одно значение float
	void SetFloat(float begin, const char* var);

	//метод-геттер для хэндлера шейдерной программы в памяти видеокарты
	GLuint GetProgramID();

private:
	//Хэндлер шейдерной программы в памяти видеокарты
	GLuint programID;
	//Готова ли программа к использованию
	bool linked;
};