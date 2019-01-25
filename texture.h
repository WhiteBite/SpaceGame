#pragma once
#include <glad/glad.h>
#include <iostream>
#include <FreeImage.h>

//Абстрактный класс текстуры
class CTexture
{
public:
	CTexture() = default;

	virtual ~CTexture() {};

	//Выбрать новую текстуру. Передается имя файла с текстурой и параметры для отрисовки
	virtual bool SetTexture(const char *path, GLuint params = 0) { return false; };
	//Загрузить текстуру из файла
	virtual bool LoadTexture(GLenum image_format,
		GLint internal_format ,GLint level, GLint border) {return false;};
	//Метод-геттер для получения массива байт, описывающего изображение
	virtual void* pixels() { return nullptr; };

	//Загружена ли текстура
	bool isLoaded = false;
	//Хэндлер текстуры в памяти видеокарты
	GLuint gl_texID = 0;
	//Количество сторон в атлассе
	GLuint sides = 1;
	//Количество кадров 2Д анимации в атлассе
	unsigned int cadres = 1;

protected:
	//Имя файла с изображением
	std::string texture_path;
};

//Плоская текстура
class CTexture2D : public CTexture
{
public:

	explicit CTexture2D(const char* path, GLuint sides = 1, GLint cadres = 1, GLuint params = 0);
	~CTexture2D();

	bool SetTexture(const char *path, GLuint params = 0);
	bool LoadTexture(GLenum image_format = GL_RGBA,		//format the image is in
		GLint internal_format = GL_RGBA,		//format to store the image in
		GLint level = 0,					//mipmapping level
		GLint border = 0);

};

//Кубическая текстура
class CTexture3D : public CTexture
{
public:

	explicit CTexture3D(const char* path, GLuint sides = 1, GLint cadres = 1, GLuint params = 0);
	~CTexture3D();

	bool SetTexture(const char *path, GLuint params = 0);
	bool LoadTexture(GLint side , GLenum image_format = GL_RGBA,		//format the image is in
		GLint internal_format = GL_RGBA,		//format to store the image in
		GLint level = 0,					//mipmapping level
		GLint border = 0);

};
