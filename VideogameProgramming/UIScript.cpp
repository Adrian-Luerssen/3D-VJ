#include "UIScript.h"
#include <chrono>

void UIScript::startScript() {

}

void UIScript::tickScript(float deltaTime) {
	

	if (glfwGetKey(window,GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		if (!firstClick) {
			ComponentHandle<GameController> game;
			world->each<GameController>([&](Entity* ent, ComponentHandle<GameController> gameController) {game = gameController; });
			game->freeCam = !game->freeCam;
			firstClick = true;
		}
		
	}
	else {
		firstClick = false;
	}
}