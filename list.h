#pragma once
#include <iostream>
#include "shaders.h"

//Структура вершины списка
struct Node
{
	//Имя вершины
	std::string name;
	//Конструктор вершины, получающий имя вершины и шейдерную программу
	Node(std::string name) { this->name = name; shp = new CShaderProgram(new CShader(name + ".vert", GL_VERTEX_SHADER), new CShader(name + ".frag", GL_FRAGMENT_SHADER)); }
	//Шейдерная программа (Хранимое в вершине значение)
	CShaderProgram* shp;
	//Указатели на следующий и предыдущий элементы списка
	Node* next = nullptr;
	Node* prev = nullptr;
};

//Линейный двусвязный список
class List
{
protected:
	//Первый элемент
	Node* first = nullptr;
	//Последний элемент
	Node* last = nullptr;
	//Текущий элемент
	Node* current = nullptr;
	//Размер списка
	unsigned int size = 0;

public:
	~List();
	//Поместить новый элемент в начало списка
	virtual void push_front(std::string name);
	//Поместить новый элемент в конец списка
	virtual void push_back(std::string name);
	//Сдвинуть указатель на текущий элемент вперед по списку
	virtual void get_next() { if (current->next)current = current->next; };
	//Сдвинуть указатель на текущий элемент назад по списку
	virtual void get_prev() { if (current->prev)current = current->prev; };
	//Убрать элемент из начала списка
	virtual Node* pop_front();
	//Убрать элемент из конца списка
	virtual Node* pop_back();
	//Удалить элемент по указателю
	virtual bool del(Node* ptr);
	//Найти элемент по имени вершины
	virtual bool find(std::string name);
	//Функция-геттер для хранимой в текущем элементе шейдерной программы
	CShaderProgram* get_current() { if (current)return current->shp; return nullptr; }
	//Аналогичный оператор
	CShaderProgram* operator[](int i);

};