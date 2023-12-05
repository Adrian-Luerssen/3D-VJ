#include "AsteroidScript.h"
#include <cmath>




void AsteroidScript::tickScript(float deltaTime) {
	float renderDistance = 5000;
	glm::vec3 userPos = glm::vec3(0);
	world->each<UserComponent>([&](Entity* ent, ComponentHandle<UserComponent> userComp) {
		renderDistance = userComp->renderDistance;
		userPos = ent->get<Camera>()->position;
		});
	renderDistance += renderDistance / 10;

	int currLevel = 2;
	world->each<EnemyComponent>([&](Entity* entity, ComponentHandle<EnemyComponent> enemyComp) {
		if (!enemyComp->spawned) {
			generateStartPos(1, renderDistance, userPos);
			enemyComp->startPos = startPos;
			enemyComp->dir = dir;
			entity->get<Transform3D>()->position = startPos;
			enemyComp->spawned = true;
		}
		else {
			ComponentHandle<Transform3D> transform = entity->get<Transform3D>();



			glm::vec3 entityPos = transform->position;

			// Calculate the distance between the user and the entity
			float distance = glm::length(entityPos - userPos);

			if (distance > renderDistance) {
				std::cout << "Out of bounds = " << entity->get<EnemyComponent>()->id << std::endl;
				world->destroy(entity);
			}


			ComponentHandle<CubeCollider> collider = entity->get<CubeCollider>();
			ComponentHandle<EnemyComponent> enemy = entity->get<EnemyComponent>();

			transform->position += (enemyComp->dir * deltaTime / 10.f);
			//transform->rotation += deltaTime / 3.f;
			if (!(hp <= 0)) {
				//transform->rotation += deltaTime / 12.f;
			}

			//cout << "change: " << this->dirX<<", "<<this->dirY << endl;


			if (collider->collidedWith) {
				//cout << "collision" << endl;
				hp--;
				//collider->collidedWith = false;

				if (hp == 0) {
					destroyed = false;
					hp--;
					world->destroy(entity);
					/*world->each<ScoreComponent>([&](Entity* other_ent, ComponentHandle<ScoreComponent> score_component) {
						//cout << "Score: " << score_component->score << endl;
						if (enemy->destroyed) {
							score_component->score += enemy->points;
							cout << "entities dest by bullet id = " << entity->get<EnemyComponent>()->id << endl;
						}
						else {
							cout << "entities dest by ship id = " << entity->get<EnemyComponent>()->id << endl;
						}
						score_component->entitiesDestroyed++;

						enemy->destroyed = true;
					});
					entity->get<SoundComponent>()->playSound = true;*/
					//this->score += enemy->points;

				}
			}
		}
		});
	

}

void AsteroidScript::generateStartPos(int level, float renderDistance, glm::vec3& userPos) {
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

