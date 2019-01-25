#pragma once
#include <cstdlib>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "world.h"
#include "list.h"

#undef main;

enum STATES {MAIN_MENU = 0, GAME_RUNNING, GAME_OVER};

//Размеры экрана по умолчанию
unsigned int SCR_WIDTH = 1280;
unsigned int SCR_HEIGHT = 720;

//Аккумулятор времени выполнения с момента прошлой смены кадров
double accumulator = 0.0, lastFrame = 0.0;
//Время, выделенное на 1 кадр
float timeStep = 1.0f / 60.0f;
//Номер текущего кадра
GLint frame = 0;

int killed_mobs = 0;

//Инициализация всех подсистем
int Init();
//Очистка памяти
void Quit();
//Обновление окна с перерисовкой кадра
void Update();
//Ввод с клавиатуры
void KeyInput(GLFWwindow* window, int key, int scancode, int action, int mods);
//Ввод мыши
void MouseInput(GLFWwindow* window, double xpos, double ypos);
//Ввод клавиш мыши
void MouseButtonInput(GLFWwindow* window, int button, int action, int mods);
//Обработка покидания пределов окна мышью
void MouseEnterInput(GLFWwindow* window, int entered);

//Массив моделей
CModel* MODELS[M_LEN];
//Список шейдерных программ для объектов сцены
CShaderProgram* GENERAL_SHADER;
//Список шейдерных программ для скайбокса
CShaderProgram* SKYBOX_SHADER;

//Настройка демонстрационной сцены
void SetScene();

//Инициализация моделей
void InitModels();
//Очистка моделей
void UninitModels();

//Инициализация шейдерных программ
void InitModes();