#include "sides.h"

unsigned int GetSide(glm::vec3 view, glm::vec3 dir)
{
	glm::vec3 dir2D = dir;
	dir2D[1] = 0.0f;
	view[1] = 0.0f;
	view = glm::normalize(view);
	dir2D = glm::normalize(dir2D);
	float ang = glm::orientedAngle(dir2D, view, glm::vec3(0.0f, 1.0f, 0.0f));
	int side = -1;
	if (-pi_over_four < ang && ang < pi_over_four)
	{
		side = BACK;
	}
	else if (-three_pi_over_four > ang || ang > three_pi_over_four)
	{
		side = FRONT;
	}
	else if (ang > 0)
	{
		side = LEFT;
	}
	else
	{
		side = RIGHT;
	}
	return side;
}