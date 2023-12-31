#include "FirstPersonCameraScript.h"
#include <chrono>

void FirstPersonCameraScript::startScript() {

}

void FirstPersonCameraScript::tickScript(float deltaTime) {

	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		speed = 0.2f;
	}
	else {
		speed = 0.1f;
	}
	float speedDelta = speed * deltaTime;

	float width = 800;
	float height = 800;

	ComponentHandle<Camera> cam = entity->get<Camera>();
	glm::vec3 currentPosition = cam->eye;
	glm::vec3 desiredPosition = cam->eye;
	ComponentHandle<GameController> game;
	world->each<GameController>([&](Entity* ent, ComponentHandle<GameController> gameController) {game = gameController; });
	if (!game->freeCam) {
		return;
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		desiredPosition += speedDelta * cam->front;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		desiredPosition += speedDelta * -glm::normalize(glm::cross(cam->front, cam->up));
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		desiredPosition += speedDelta * -cam->front;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		desiredPosition += speedDelta * glm::normalize(glm::cross(cam->front, cam->up));
	}
	


	
	cam->eye = desiredPosition;
	//cam->position.y += eyeLevel;


	// Handles mouse inputs
	if (!game->pause)
	{
		// Hides mouse cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Prevents camera from jumping on the first click
		if (firstClick)
		{
			glfwSetCursorPos(window, (width / 2), (height / 2));
			firstClick = false;
		}

		// Stores the coordinates of the cursor
		double mouseX;
		double mouseY;
		// Fetches the coordinates of the cursor
		glfwGetCursorPos(window, &mouseX, &mouseY);

		// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
		// and then "transforms" them into degrees 
		float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

		// Calculates upcoming vertical change in the Orientation
		glm::mat4 m = glm::mat4(1.0f);

		m = glm::rotate(m, glm::radians(-rotX), glm::normalize(glm::cross(cam->front, cam->up)));

		// Rotates the Orientation left and right
		glm::mat4 m2 = glm::mat4(1.0f);

		m2 = glm::rotate(m2, glm::radians(-rotY), cam->up);

		cam->front = m * m2 * glm::vec4(cam->front, 1.);


		// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
		glfwSetCursorPos(window, (width / 2), (height / 2));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		// Unhides cursor since camera is not looking around anymore
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// Makes sure the next time the camera looks around it doesn't jump
		firstClick = true;
	}
	

	cam->target = cam->eye;




}