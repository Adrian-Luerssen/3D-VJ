#include "EnemyShipScript.h"
#include <cmath>




void EnemyShipScript::tickScript(float deltaTime) {
	ComponentHandle<GameController> game;
	world->each<GameController>([&](Entity* ent, ComponentHandle<GameController> gameController) {game = gameController; });
	if (game->pause) return;
	float renderDistance = 5000;
	glm::vec3 userPos = glm::vec3(0);
	world->each<Camera>([&](Entity* ent, ComponentHandle<Camera> camera) {
		renderDistance = camera->renderDistance;
		userPos = camera->eye;
		});
	renderDistance += renderDistance / 10;
	world->each<UserComponent>([&](Entity* ent, ComponentHandle<UserComponent> user) {
		userPos = ent->get<Transform3D>()->position;
		});
	
	world->each<EnemyComponent>([&](Entity* shipEntity, ComponentHandle<EnemyComponent> enemyComp) {
		if (enemyComp->name != "Ship") return;
		int shipID = enemyComp->id;
		world->each<EnemyShipCanon>([&](Entity* entity, ComponentHandle<EnemyShipCanon> enemyCanon) {
			// Calculate direction vector from cannon to user position
			entity->get<Transform3D>()->position = shipEntity->get<Transform3D>()->position + glm::vec3(0, 20, 0);
			glm::vec3 cannonToUser = userPos - entity->get<Transform3D>()->position;

			// Use lookAt to calculate rotation matrix
			glm::mat4 rotationMatrix = glm::lookAt(glm::vec3(0.0f), cannonToUser, glm::vec3(0.0f, 1.0f, 0.0f));

			// Extract pitch and yaw from the rotation matrix
			float pitch = asin(-rotationMatrix[1][2]);
			float yaw = atan2(rotationMatrix[0][2], rotationMatrix[2][2]);

			// Set the rotation of the enemy cannon
			entity->get<Transform3D>()->rotation = glm::vec3(pitch, yaw, 0.0f);
			});



		});

	
	
}

void EnemyShipScript::generateStartPos(int level, float renderDistance, glm::vec3& userPos) {
	int face = std::rand() % 6; // Select a random face of the cube
	float other1 = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * 2.0f - 1.0f;
	float other2 = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * 2.0f - 1.0f;

	renderDistance /= 2;
	// Generate a random coordinate on the selected face
	glm::vec3 startPos;
	switch (face) {
	case 0: // Front face
		startPos = glm::vec3(userPos.x + (other1 * renderDistance), userPos.y + (other2 * renderDistance), userPos.z + renderDistance);
		break;
	case 1: // Back face
		startPos = glm::vec3(userPos.x + (other1 * renderDistance), userPos.y + (other2 * renderDistance), userPos.z - renderDistance);
		break;
	case 2: // Top face
		startPos = glm::vec3(userPos.x + (other1 * renderDistance), userPos.y + renderDistance, userPos.z + (other2 * renderDistance));
		break;
	case 3: // Bottom face
		startPos = glm::vec3(userPos.x + (other1 * renderDistance), userPos.y - renderDistance, userPos.z + (other2 * renderDistance));
		break;
	case 4: // Right face
		startPos = glm::vec3(userPos.x + renderDistance, userPos.y + (other1 * renderDistance), userPos.z + (other2 * renderDistance));
		break;
	case 5: // Left face
		startPos = glm::vec3(userPos.x - renderDistance, userPos.y + (other1 * renderDistance), userPos.z + (other2 * renderDistance));
		break;
	}

	// Calculate the direction vector pointing towards the center
	glm::vec3 dir = glm::normalize(userPos - startPos);
	// Add some randomness to the direction vector
	float randomnessFactor = 0.1f; // You can adjust this value based on how much randomness you want
	dir.x += (static_cast<float>(std::rand()) / RAND_MAX - 0.5f) * randomnessFactor;
	dir.y += (static_cast<float>(std::rand()) / RAND_MAX - 0.5f) * randomnessFactor;
	dir.z += (static_cast<float>(std::rand()) / RAND_MAX - 0.5f) * randomnessFactor;

	// Normalize the direction vector again to maintain unit length
	dir = glm::normalize(dir);
	// Set the member variables
	this->startPos = startPos;
	this->dir = dir;

	// cout << "startPos: " << this->startPos.x << ", " << this->startPos.y << ", " << this->startPos.z << endl;
	// cout << "direction: " << this->dir.x << ", " << this->dir.y << ", " << this->dir.z << endl;
	this->ticksBeforeAppear = rand() % (10000 / level);
}



void EnemyShipScript::spawnShards(ComponentHandle<Transform3D> transform) {
	

}