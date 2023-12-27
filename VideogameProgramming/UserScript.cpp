#include "UserScript.h"
#include <cmath>


template<typename T>
T lerp(const T& start, const T& end, float t) {
    return start + t * (end - start);
}


void UserScript::tickScript(float deltaTime) {
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		speed = 0.3f;
	}
	else {
		speed = 0.15f;
	}
	float speedDelta = speed * deltaTime;

	float width = 800;
	float height = 800;
	ComponentHandle<Camera> cam;
	world->each<Camera>([&](Entity* ent, ComponentHandle<Camera> camera) {
		cam = camera;
		});
	ComponentHandle<GameController> game;
	world->each<GameController>([&](Entity* ent, ComponentHandle<GameController> gameController) {game = gameController; });
	if (game->freeCam) {
		return;
	}
	ComponentHandle<UserComponent> user = entity->get<UserComponent>();
	ComponentHandle<CubeCollider> userCollider = entity->get<CubeCollider>();
	ComponentHandle<Transform3D> transf = entity->get<Transform3D>();
	glm::vec3 currentEye = transf->position;
	glm::vec3 desiredEye = transf->position;

	/*glm::mat4 rotationMatrix =
		glm::rotate(glm::mat4(1.0f), glm::radians(transf->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::rotate(glm::mat4(1.0f), glm::radians(transf->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)) *
		glm::rotate(glm::mat4(1.0f), glm::radians(transf->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));*/
	glm::mat4 rotationMatrix =
		glm::rotate(glm::mat4(1.0f), transf->rotation.y, glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::rotate(glm::mat4(1.0f), transf->rotation.x, glm::vec3(1.0f, 0.0f, 0.0f)) *
		glm::rotate(glm::mat4(1.0f), transf->rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

	glm::vec3 forward = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)));
	glm::vec3 right = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f)));
	glm::vec3 up = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f)));

	float speedFactor = 400.0f;
	if (!game->pause)
	{
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			desiredEye -= speedDelta * forward;
			if (speed == 0.15f) {
				if (user->thrust >= 0.5 && user->thrust < 0.6) {
					user->thrust = 0.5;
				}
				else if (user->thrust > 0.5) {
					user->thrust -= deltaTime / speedFactor;
				}
				else {
					user->thrust += deltaTime / speedFactor;

				}
			}
			else {
				if (user->thrust >= 1) {
					user->thrust = 1;
				}
				else {
					user->thrust += deltaTime / speedFactor;
				}
			}
			//cout << "user thrust: " << user->thrust << endl;
		}
		else {
			user->thrust -= deltaTime / speedFactor;
			if (user->thrust < 0) {
				user->thrust = 0;
			}
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			desiredEye += speedDelta * right;

		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			desiredEye += speedDelta * forward;
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			desiredEye -= speedDelta * right;
		}
		// Set the camera's up vector to the object's up vector
		cam->up = up;

		bool col = false;

		world->each<EnemyComponent>([&](Entity* ent, ComponentHandle<EnemyComponent> enemy) {
			if (col) return;
			if (game->imunity) return;
			ComponentHandle<Transform3D> enemyTransform = ent->get<Transform3D>();
			ComponentHandle<CubeCollider> enemyCollider = ent->get<CubeCollider>();
			if (enemyCollider == NULL) return;

			// Check for collision along the X-axis
			bool collisionX = transf->position.x + userCollider->width >= enemyTransform->position.x - enemyCollider->width &&
				enemyTransform->position.x + enemyCollider->width >= transf->position.x - userCollider->width;

			// Check for collision along the Y-axis
			bool collisionY = transf->position.y + userCollider->height >= enemyTransform->position.y - enemyCollider->height &&
				enemyTransform->position.y + enemyCollider->height >= transf->position.y - userCollider->height;

			// Check for collision along the Z-axis
			bool collisionZ = transf->position.z + userCollider->length >= enemyTransform->position.z - enemyCollider->length &&
				enemyTransform->position.z + enemyCollider->length >= transf->position.z - userCollider->length;

			// If there is a collision along all axes, then a collision occurred
			if (collisionX && collisionY && collisionZ) {
				// Collision happened, you can handle it here
				enemy->destroyed = true;
				game->lives--;
				game->imunity = true;
				entity->get<SoundComponent>()->playSound = true;
				col = true;
				cout << "hit by "<<enemy->name<< " only have "<<game->lives <<" left" << endl;
			}
			});

		//if (desiredEye.y < -5) {
		//	cout << "fallen out of world" << endl;
		//	desiredEye.y = 20;
		//}
		//user->eye.y = desiredEye.y;
		//cout << "pos: " << user->eye.x << ", " << user->eye.y << ", " << user->eye.z << endl;

		transf->position = lerp(transf->position, desiredEye, 0.5);
		//user->eye.y += eyeLevel;


		// Handles mouse inputs
		// Hides mouse cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Prevents user from jumping on the first click
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

		// Update rotation angles
		user->ayaw -= rotY;
		user->apitch += rotX;

		// Clamp pitch to avoid over-rotation
		if (user->apitch > 89.0f)
			user->apitch = 89.0f;
		if (user->apitch < -89.0f)
			user->apitch = -89.0f;

		// Update entity's rotation
		transf->rotation.x = glm::radians(user->apitch);
		transf->rotation.y = glm::radians(user->ayaw);

		// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
		glfwSetCursorPos(window, (width / 2), (height / 2));

		// Set the camera's target to the object's position
		cam->target = transf->position;

		// Calculate the camera's position based on the object's rotation (pitch and yaw)
		float distance = 100.0f; // Adjust the distance as needed
		float heightOffset = 20.0f; // Adjust the height offset as needed
		glm::vec3 offset = glm::vec3(0.0f, heightOffset, -(distance + (user->thrust * 25))); // Adjust the offset values
		rotationMatrix = glm::rotate(glm::mat4(1.0f), transf->rotation.y, glm::vec3(0, 1, 0));
		rotationMatrix = glm::rotate(rotationMatrix, transf->rotation.x, glm::vec3(1, 0, 0));
		glm::vec3 desiredEye = transf->position + glm::vec3(rotationMatrix * glm::vec4(offset, 1.0f));
		
		cam->eye = desiredEye;
		// Assuming you want the camera to always face the back of the object
		cam->front = -forward;
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		// Unhides cursor since userera is not looking around anymore
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// Makes sure the next time the userera looks around it doesn't jump
		firstClick = true;
	}




}
