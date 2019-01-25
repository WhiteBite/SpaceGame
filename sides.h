#pragma once
#include "glmaths.h"

//Перечисление сторон 3Д модели
enum SIDES { FRONT = 0, BACK, RIGHT, LEFT };

//Метод для определения стороны модели, которой она повернута к камере
unsigned int GetSide(glm::vec3 view, glm::vec3 dir);

// pi/4
const float pi_over_four = glm::quarter_pi<float>();
// 3*pi/4
const float three_pi_over_four = 3.0f * glm::quarter_pi<float>();