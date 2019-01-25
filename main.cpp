#include "main.h"

#define _CRTDBG_MAP_ALLOC 
#include <stdlib.h> 
#include <crtdbg.h>  

int STATE = 0;
bool running;
GLFWwindow* WINDOW;
CCamera* CAMERA;
CWorld WORLD;
CEntity* sun;
CEntity* fuel;
CEntity* hud;
CEntity* menu;
int ENERGY = 1000;
unsigned int consumption = 1;
int cyc = 0;
double lastMouseX, lastMouseY;
glm::vec3 spawns[4] = { glm::vec3(50.0f, 0.0f, 50.0f), glm::vec3(50.0f, 0.0f, 0.0f), glm::vec3(-50.0f, 0.0f, 50.0f), glm::vec3(0.0f, 0.0f, 50.0f) };

int main(int argc, char** argv)
{
	if (Init())
	{
		return 1;
	}
	SetScene();

	running = true;

	while (!glfwWindowShouldClose(WINDOW) && running)
	{
		Update();
		glfwPollEvents();
	}

	Quit();
	return 0;
}

void SetScene()
{
	CAMERA = new CCamera();
	WORLD.SetScreenSize(SCR_WIDTH, SCR_HEIGHT);

	sun = new CEntity(M_BILLBOARD);
	sun->SetTexture("sun");
	sun->SetScale(glm::vec3(25.0f, 25.0f, 25.0f));
	sun->MakeHud();
	WORLD.AddEntity(sun);

	CAMERA->SetPos(glm::vec3(0.0f, 0.0f, 0.0f));
	CAMERA->SetMoving(5.0f);

	hud = new CEntity(M_BILLBOARD);
	hud->SetTexture("hud");
	hud->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
	hud->MakeHud();
	WORLD.AddEntity(hud);

	fuel = new CEntity(M_BILLBOARD);
	fuel->SetTexture("fuel");
	fuel->MakeHud();
	WORLD.AddEntity(fuel);

	menu = new CEntity(M_BILLBOARD);
	menu->SetTexture("menu");
	menu->MakeHud();
	WORLD.AddEntity(menu);
	STATE = MAIN_MENU;
	killed_mobs = 0;
	ENERGY = 1000;

	WORLD.skybox = new CSkybox("000");
}

void PlaceBomb()
{
	CEntity* bomb = new CEntity(M_BILLBOARD);
	bomb->SetTexture("bomb");
	bomb->SetAnimated(2);
	bomb->PlayAnimation();
	bomb->SetPosition(CAMERA->GetPos() - (2.0f*CAMERA->GetDir()));
	WORLD.AddEntity(bomb);
	ENERGY -= 50;
	if (ENERGY <= 0)
	{
		ENERGY = 0;
		CAMERA->SetMoving(1.0f);
	}
}

void PlaceShip()
{
	CEntity* ship = new CEntity(M_BILLBOARD);
	ship->SetTexture("ship");
	ship->SetPosition(CAMERA->GetPos() + spawns[rand() % 4]);
	ship->MakeMovable();
	WORLD.AddEntity(ship);
}

void Restart()
{
	WORLD.Restart();
	SetScene();
}

void Update()
{
	double time = glfwGetTime();
	double delta = time - lastFrame;
	lastFrame = time;
	accumulator += delta;
	std::string energy;
	while (accumulator >= timeStep)
	{
		frame++;
		if (STATE == GAME_RUNNING)
		{
			if (frame % 150 == 0)
				PlaceShip();

			energy = std::to_string(killed_mobs);
			glfwSetWindowTitle(WINDOW, energy.c_str());

			glm::vec3 dir = CAMERA->GetDir2D();
			glm::vec3 pos = glm::vec3(1.0f, 0.0f, 1.0f);
			int side = GetSide(pos, dir);
			if (side == SIDES::LEFT || side == SIDES::RIGHT)
				ENERGY += 3;

			sun->SetPosition(CAMERA->GetPos() + glm::vec3(30.0f, 0.0f, 30.0f));
			glm::vec3 huds = CAMERA->GetPos() + CAMERA->GetDir();
			hud->SetPosition(huds);
			fuel->SetPosition(huds);
			fuel->SetScale(glm::vec3((ENERGY / 1000.0f) / 10.0f, 0.45f, 1.0f));

			ENERGY -= consumption;
			if (ENERGY <= 0)
			{
				ENERGY = 0;
				CAMERA->SetMoving(1.0f);
				STATE = GAME_OVER;
				menu->SetTexture("gameover");
				WORLD.UpdateWorld(CAMERA, frame);
			}
			if (ENERGY > 1000)
				ENERGY = 1000;
			CAMERA->Update(0.05f);
		}
		else 
		{
			menu->SetPosition(CAMERA->GetPos() + 1.5f*CAMERA->GetDir());
			CAMERA->Update(0.0f);
		}
		

		if (!WORLD.UpdateWorld(CAMERA, frame) && STATE == GAME_RUNNING)
		{
			STATE = GAME_OVER;
			menu->SetTexture("gameover");
			WORLD.UpdateWorld(CAMERA, frame);
		}
		glFlush();
		glfwSwapBuffers(WINDOW);

		accumulator -= timeStep;
	}

}

void KeyInput(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		if (STATE == MAIN_MENU)
		{
			menu->SetTexture("blank");
			STATE = GAME_RUNNING;
		}
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			running = false;
			break;
		case GLFW_KEY_LEFT_SHIFT:
			CAMERA->SetMoving(10.0f);
			CAMERA->SetFov(CAMERA->GetFov() + 10.0f);
			consumption += 3;
			break;
		case GLFW_KEY_S:
			CAMERA->SetMoving(2.0f);
			CAMERA->SetFov(CAMERA->GetFov() - 10.0f);
			consumption -= 1;
			break;
		case GLFW_KEY_SPACE:
			PlaceBomb();
			break;
		case GLFW_KEY_R:
			Restart();
			break;
		}
	}
	else if (action == GLFW_RELEASE)
	{
		switch (key) 
		{
		case GLFW_KEY_LEFT_SHIFT:
			CAMERA->SetMoving(5.0f);
			CAMERA->SetFov(CAMERA->GetFov() - 10.0f);
			consumption -= 3;
			break;
		case GLFW_KEY_S:
			CAMERA->SetMoving(5.0f);
			CAMERA->SetFov(CAMERA->GetFov() + 10.0f);
			consumption += 1;
			break;
		}
	}
}

void MouseInput(GLFWwindow* window, double xpos, double ypos)
{

	float mouseX = (float)xpos - lastMouseX;
	float mouseY = (float)ypos - lastMouseY;

	if(ENERGY)
		//CAMERA->RotateDirection(-0.5*mouseX, -0.5*mouseY);
		CAMERA->RotateDirection(-0.5*mouseX, 0.0f);

	lastMouseX = xpos;
	lastMouseY = ypos;
}

void MouseButtonInput(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		if (button == GLFW_MOUSE_BUTTON_RIGHT)
		{
			std::cout << frame << "\n";
			std::cout << CAMERA->GetPos()[0] << " " << CAMERA->GetPos()[1] << " " << CAMERA->GetPos()[2] << "\n";
			std::cout << CAMERA->GetDir2D()[0] << " " << CAMERA->GetDir2D()[1] << " " << CAMERA->GetDir2D()[2] << "\n";
		}
	}
}

void MouseEnterInput(GLFWwindow* window, int entered)
{
	if (!entered)
	{
		glfwSetCursorPos(WINDOW, SCR_WIDTH/2, SCR_HEIGHT/2);
		lastMouseX = SCR_WIDTH / 2;
		lastMouseY = SCR_HEIGHT / 2;
	}
}

int Init()
{
	if (!glfwInit())
		return -1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	WINDOW = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Shader Tester", NULL, NULL);
	if (!WINDOW)
	{
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(WINDOW);

	glfwSetKeyCallback(WINDOW, KeyInput);
	glfwSetCursorPosCallback(WINDOW, MouseInput);
	glfwSetCursorPos(WINDOW, SCR_WIDTH / 2.0, SCR_WIDTH / 2.0);
	glfwSetMouseButtonCallback(WINDOW, MouseButtonInput);
	glfwSetCursorEnterCallback(WINDOW, MouseEnterInput);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	InitModes();
	InitModels();

	glClearColor(0.0f, 0.5f, 1.0f, 1.0f);
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_ALPHA);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	std::cout << "Type 'help' or '?' to see the full list of commands:\n";

	return 0;
}

void Quit()
{
	UninitModels();
	
	glfwTerminate();
}

void InitModels()
{
	MODELS[M_SMART_SPRITE] = new CModel("smart-sprite", M_SMART_SPRITE);
	MODELS[M_BILLBOARD] = new CModel("billboard", M_BILLBOARD);
	MODELS[M_FULLSCREEN] = new CModel("fullscreen", M_FULLSCREEN);
	MODELS[M_SKYBOX] = new CModel("skybox");
}

void UninitModels() {
	for (auto &i : MODELS)
		delete i;
}

void InitModes()
{
	GENERAL_SHADER = new CShaderProgram(new CShader("gen_basic.vert", GL_VERTEX_SHADER), new CShader("gen_basic.frag", GL_FRAGMENT_SHADER));
	SKYBOX_SHADER = new CShaderProgram(new CShader("sky_parallax.vert", GL_VERTEX_SHADER), new CShader("sky_parallax.frag", GL_FRAGMENT_SHADER));
}
