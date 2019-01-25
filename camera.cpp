#include "camera.h"

CCamera::CCamera()
{
	fov = 60.0f;
	current_fov = 30.0f;
	position = glm::vec3(1.0f, 0.0f, 1.0f);
	direction = glm::vec3(0.0f, 0.0f, 1.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	m_projection = glm::perspective(glm::radians(fov), 4.0f / 3.0f, 0.1f, 100.0f);
}

glm::mat4 CCamera::Look()
{
	return m_view;
}

void CCamera::Rotate(float amount, const glm::vec3 &axis)
{
	direction = glm::rotate(direction, amount, axis);
}

void CCamera::Translate(const glm::vec3 &direction)
{
	position += direction;
}

void CCamera::Update(float speed)
{
	Move(speed);
	Strafe(speed);
	if (current_fov != fov)
	{
		if (current_fov < fov)
			current_fov += 1.0f;
		else
			current_fov -= 1.0f;
		m_projection = glm::perspective(glm::radians(current_fov), 4.0f / 3.0f, 0.1f, 100.0f);
	}
	m_view = glm::lookAt(position, position + direction, up);
}

glm::vec3 CCamera::GetDir()
{ 
	return direction; 
}

glm::vec3 CCamera::GetDir2D() 
{ 
	glm::vec3 cam = direction; 
	cam[1] = 0.0f; 
	return glm::normalize(cam); 
}

glm::vec3 CCamera::GetUp() 
{ 
	return up; 
}

glm::vec3 CCamera::GetPos() 
{ 
	return position; 
}

void CCamera::SetPos(glm::vec3 pos) 
{ 
	position = pos; 
}

void CCamera::SetDir(glm::vec3 dir) 
{ 
	direction = dir; 
}

void CCamera::SetDir2D(glm::vec3 dir) 
{ 
	direction = dir; dir[1] = 0.0f; 
}

void CCamera::SetFov(float fov_) 
{ 
	fov = fov_; 
}

float CCamera::GetFov() 
{ 
	return fov; 
}

void CCamera::RotateDirection(float mouseX, float mouseY)
{
	mouseX *= sensetivity;
	mouseY *= sensetivity;
	direction = glm::rotate(direction, mouseY, glm::cross(direction, up));
	direction = glm::rotate(direction, mouseX, glm::cross(direction, glm::cross(up, direction)));
}

void CCamera::Move(float speed)
{
	position += moving[0] * speed * direction;
}

void CCamera::Strafe(float speed)
{
	position += moving[1] * speed * glm::cross(up, direction);
}

void CCamera::SetMoving(float m)
{
	moving[0] = m;
}
void CCamera::SetStrafing(float m)
{
	moving[1] = m;
}