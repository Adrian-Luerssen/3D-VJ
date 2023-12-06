#include "UserScript.h"
#include <cmath>




void UserScript::tickScript(float deltaTime) {
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
	ComponentHandle<Camera> cam;
	world->each<Camera>([&](Entity* ent, ComponentHandle<Camera> camera) {
		cam = camera;
		});

	ComponentHandle<UserComponent> user = entity->get<UserComponent>();
	ComponentHandle<Transform3D> transf = entity->get<Transform3D>();
	glm::vec3 currentEye = transf->position;
	glm::vec3 desiredEye = transf->position;

	// Assuming transf->rotation is in radians
	glm::mat4 rotationMatrix =
		glm::rotate(glm::mat4(1.0f), transf->rotation.y, glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::rotate(glm::mat4(1.0f), transf->rotation.x, glm::vec3(1.0f, 0.0f, 0.0f)) *
		glm::rotate(glm::mat4(1.0f), transf->rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

	glm::vec3 forward = glm::vec3(rotationMatrix * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
	glm::vec3 right = glm::vec3(rotationMatrix * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));

	/*if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		desiredEye -= speedDelta * forward;
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
	}*/

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		desiredEye += speedDelta * cam->front;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		desiredEye += speedDelta * -glm::normalize(glm::cross(cam->front, cam->up));
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		desiredEye += speedDelta * -cam->front;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		desiredEye += speedDelta * glm::normalize(glm::cross(cam->front, cam->up));
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		moveCam = false;
	}
	


	bool col = false;

	world->each<EnemyComponent>([&](Entity* ent, ComponentHandle<EnemyComponent> enemy) {
		if (col) return;
		ComponentHandle<Transform3D> enemyTransform = ent->get<Transform3D>();
		ComponentHandle<CubeCollider> enemyCollider = ent->get<CubeCollider>();

		// Check for collision along the X-axis
		bool collisionX = desiredEye.x >= enemyTransform->position.x - enemyCollider->width &&
			enemyTransform->position.x + enemyCollider->width >= desiredEye.x;

		// Check for collision along the Y-axis
		bool collisionY = desiredEye.y >= enemyTransform->position.y - enemyCollider->height &&
			enemyTransform->position.y + enemyCollider->height >= desiredEye.y;

		// Check for collision along the Z-axis
		bool collisionZ = desiredEye.z >= enemyTransform->position.z - enemyCollider->length &&
			enemyTransform->position.z + enemyCollider->length >= desiredEye.z;

		// If there is a collision along all axes, then a collision occurred
		if (collisionX && collisionY && collisionZ) {
			// Collision happened, you can handle it here
			cout << "lives --" << endl;
			ent->getWorld()->destroy(ent);
			col = true;
		}
		});

	//if (desiredEye.y < -5) {
	//	cout << "fallen out of world" << endl;
	//	desiredEye.y = 20;
	//}
	//user->eye.y = desiredEye.y;
	//cout << "pos: " << user->eye.x << ", " << user->eye.y << ", " << user->eye.z << endl;

	transf->position = desiredEye;
	//user->eye.y += eyeLevel;


	// Handles mouse inputs
	if (moveCam)
	{
		// Hides mouse cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Prevents userera from jumping on the first click
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

		/*// Calculates upcoming vertical change in the Orientation
		glm::mat4 m = glm::mat4(1.0f);

		m = glm::rotate(m, glm::radians(-rotX), glm::normalize(glm::cross(user->front, user->up)));

		// Rotates the Orientation left and right
		glm::mat4 m2 = glm::mat4(1.0f);

		m2 = glm::rotate(m2, glm::radians(-rotY), user->up);

		user->front = m * m2 * glm::vec4(user->front, 1.);*/
		// Calculates the rotation matrix based on mouse input
		user->ayaw += rotY;
		user->apitch -= rotX;
		/**/if (user->apitch > 89.0f)
			user->apitch = 89.0f;	// doesnt allow the user to over rotate
		if (user->apitch < -89.0f)
			user->apitch = -89.0f;
		if (user->ayaw > 180.0f)
			user->ayaw = user->ayaw;	// doesnt allow the user to over rotate
		if (user->ayaw < -179.0f)
			user->ayaw = 180.0f;
		cout << "CAM YAW " << user->ayaw << endl;
		cout << "CAM PITCH " << user->apitch << endl;

		transf->rotation.x = user->apitch;
		transf->rotation.y = user->ayaw;

		//glm::vec3 direction = glm::vec3(cos(glm::radians(user->ayaw)) * cos(glm::radians(user->apitch)), sin(glm::radians(user->apitch)), sin(glm::radians(user->ayaw)) * cos(glm::radians(user->apitch)));
		//transf->rotation = normalize(direction);


		//cout << "pos: " << transf->rotation.x << ", " << transf->rotation.y << ", " << transf->rotation.z << endl;
		//transf->rotation = normalize(transf->rotation);
		// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
		glfwSetCursorPos(window, (width / 2), (height / 2));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		// Unhides cursor since userera is not looking around anymore
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// Makes sure the next time the userera looks around it doesn't jump
		firstClick = true;
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		//cout << "SHOOT" << endl;
		moveCam = true;
	}

	world->each<Camera>([&](Entity* ent, ComponentHandle<Camera> cam) {
		// Set the camera's target to the object's position
		cam->target = transf->position;

		// Calculate the camera's position based on the object's rotation (pitch and yaw)
		float distance = 100.0f; // Adjust the distance as needed
		glm::vec3 offset = glm::vec3(0.0f, 0.0f, distance);
		glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(transf->rotation.y), glm::vec3(0, 1, 0));
		rotationMatrix = glm::rotate(rotationMatrix, glm::radians(transf->rotation.x), glm::vec3(1, 0, 0));
		cam->eye = transf->position - glm::vec3(rotationMatrix * glm::vec4(offset, 1.0f));

		// Assuming you want the camera to always face the back of the object
		cam->front = glm::normalize(transf->position - cam->eye);
		});


}
