#pragma once

#include <iostream>
#include <queue>
#include <string>
#include "entity.h"

//Структура вершины дерева поиска
struct sNode
{
	//Ключ вершины
	float key;
	//Указатель на объект сцены(хранимое значение)
	CEntity* value;
	//Удален ли элемент
	bool deleted = false;
	//Указатели на правого и левого потомков
	sNode* right = nullptr;
	sNode* left = nullptr;
	//Высота поддерева
	int height;
	//Конструктор вершины, задающий значения по умолчанию
	sNode(CEntity* value, float key) { this->key = key; this->value = value; right = nullptr; left = nullptr; height = 1; };
};

//Бинарное дерево поиска
struct sTree
{
	//Корень дерева
	sNode* root = nullptr;
	//Высота дерева
	int levels = 0;

	sTree() { root = nullptr; levels = 0; }
	~sTree();
	//Добавить элемент в дерево
	void add(CEntity* value, float key);
	sNode* add(sNode*, CEntity* value, float key);
	//Удалить жлемент из дерева
	void del(CEntity* value);
	sNode* del(sNode* st, CEntity* value);
	//Найти элемент в дереве
	sNode* find(float key);
	//Очистить дерево
	void clear();
	void clear(sNode* st);
	//Вывести содержимое дерева
	void print();
	//Правый и левый сдвиги и балагсировка
	sNode* rotateRight(sNode* st);
	sNode* rotateLeft(sNode* st);
	sNode* balance(sNode* st);
	//Высота поддерева
	int height(sNode* st);
	//Фактор балансировки поддерева
	int bfactor(sNode* st);
	//Вспомогательные функции балансировки
	void fixHeight(sNode* st);
	sNode* findMin(sNode* st);
	sNode* removeMin(sNode* st);
	//Вывод содержимого поддерева
	void printST(sNode* st);
	//Выгрузить содержимое дерева в очередь на отрисовку
	void unload(std::vector<CEntity*>* vec);
	void unload(std::vector<CEntity*>* vec, sNode* st);
};