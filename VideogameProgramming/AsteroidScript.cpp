#include "AsteroidScript.h"
#include <cmath>




void AsteroidScript::tickScript(float deltaTime) {
	ComponentHandle<GameController> game;
	world->each<GameController>([&](Entity* ent, ComponentHandle<GameController> gameController) {game = gameController; });
	if (game->pause) return;
	float renderDistance = 5000;
	glm::vec3 userPos = glm::vec3(0);
	world->each<Camera>([&](Entity* ent, ComponentHandle<Camera> cam) {
		renderDistance = cam->renderDistance;
		userPos = cam->position;
		});
	renderDistance += renderDistance / 10;

	
	world->each<EnemyComponent>([&](Entity* entity, ComponentHandle<EnemyComponent> enemyComp) {
		if (enemyComp->destroyed) {
			// spawn shards
			spawnShards(entity->get<Transform3D>());
			world->destroy(entity);
		}
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
	
	world->each<TemporaryComponent>([&](Entity* entity, ComponentHandle<TemporaryComponent> tempComp) {
		tempComp->timeAlive += deltaTime / 12.0f;
		if (tempComp->timeAlive > tempComp->duration) {
			world->destroy(entity);
		}
		else {
			entity->get<Transform3D>()->position += tempComp->dir * deltaTime / 12.0f;
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


float getRandomFloat() {
	return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

// Function to generate a random direction vector
glm::vec3 getRandomDirection() {
	return glm::normalize(glm::vec3(static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2.0f - 1.0f, static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2.0f - 1.0f, static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2.0f - 1.0f));
}

void AsteroidScript::spawnShards(ComponentHandle<Transform3D> transform) {
	float scale = 10;
	// Shard 1
	glm::vec3 randDir1 = getRandomDirection();
	float timeAlive1 = getRandomFloat() * 300.0f;
	Entity* shard1 = world->create();
	shard1->assign<TemporaryComponent>(randDir1, timeAlive1);
	shard1->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/shards/shard1.obj");
	shard1->assign<Transform3D>(transform->position, scale);

	// Shard 2
	glm::vec3 randDir2 = getRandomDirection();
	float timeAlive2 = getRandomFloat() * 300.0f;
	Entity* shard2 = world->create();
	shard2->assign<TemporaryComponent>(randDir2, timeAlive2);
	shard2->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/shards/shard2.obj");
	shard2->assign<Transform3D>(transform->position, scale);

	// Shard 3
	glm::vec3 randDir3 = getRandomDirection();
	float timeAlive3 = getRandomFloat() * 300.0f;
	Entity* shard3 = world->create();
	shard3->assign<TemporaryComponent>(randDir3, timeAlive3);
	shard3->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/shards/shard3.obj");
	shard3->assign<Transform3D>(transform->position, scale);

	// Shard 4
	glm::vec3 randDir4 = getRandomDirection();
	float timeAlive4 = getRandomFloat() * 300.0f;
	Entity* shard4 = world->create();
	shard4->assign<TemporaryComponent>(randDir4, timeAlive4);
	shard4->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/shards/shard4.obj");
	shard4->assign<Transform3D>(transform->position, scale);

	// Shard 5
	glm::vec3 randDir5 = getRandomDirection();
	float timeAlive5 = getRandomFloat() * 300.0f;
	Entity* shard5 = world->create();
	shard5->assign<TemporaryComponent>(randDir5, timeAlive5);
	shard5->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/shards/shard5.obj");
	shard5->assign<Transform3D>(transform->position, scale);

	// Shard 6
	glm::vec3 randDir6 = getRandomDirection();
	float timeAlive6 = getRandomFloat() * 300.0f;
	Entity* shard6 = world->create();
	shard6->assign<TemporaryComponent>(randDir6, timeAlive6);
	shard6->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/shards/shard6.obj");
	shard6->assign<Transform3D>(transform->position, scale);

	// Shard 7
	glm::vec3 randDir7 = getRandomDirection();
	float timeAlive7 = getRandomFloat() * 300.0f;
	Entity* shard7 = world->create();
	shard7->assign<TemporaryComponent>(randDir7, timeAlive7);
	shard7->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/shards/shard7.obj");
	shard7->assign<Transform3D>(transform->position, scale);

	// Shard 8
	glm::vec3 randDir8 = getRandomDirection();
	float timeAlive8 = getRandomFloat() * 300.0f;
	Entity* shard8 = world->create();
	shard8->assign<TemporaryComponent>(randDir8, timeAlive8);
	shard8->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/shards/shard8.obj");
	shard8->assign<Transform3D>(transform->position, scale);

	// Shard 9
	glm::vec3 randDir9 = getRandomDirection();
	float timeAlive9 = getRandomFloat() * 300.0f;
	Entity* shard9 = world->create();
	shard9->assign<TemporaryComponent>(randDir9, timeAlive9);
	shard9->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/shards/shard9.obj");
	shard9->assign<Transform3D>(transform->position, scale);

	// Shard 10
	glm::vec3 randDir10 = getRandomDirection();
	float timeAlive10 = getRandomFloat() * 300.0f;
	Entity* shard10 = world->create();
	shard10->assign<TemporaryComponent>(randDir10, timeAlive10);
	shard10->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/shards/shard10.obj");
	shard10->assign<Transform3D>(transform->position, scale);

	// Shard 11
	glm::vec3 randDir11 = getRandomDirection();
	float timeAlive11 = getRandomFloat() * 300.0f;
	Entity* shard11 = world->create();
	shard11->assign<TemporaryComponent>(randDir11, timeAlive11);
	shard11->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/shards/shard11.obj");
	shard11->assign<Transform3D>(transform->position, scale);

	// Shard 12
	glm::vec3 randDir12 = getRandomDirection();
	float timeAlive12 = getRandomFloat() * 300.0f;
	Entity* shard12 = world->create();
	shard12->assign<TemporaryComponent>(randDir12, timeAlive12);
	shard12->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/shards/shard12.obj");
	shard12->assign<Transform3D>(transform->position, scale);

	// Shard 13
	glm::vec3 randDir13 = getRandomDirection();
	float timeAlive13 = getRandomFloat() * 300.0f;
	Entity* shard13 = world->create();
	shard13->assign<TemporaryComponent>(randDir13, timeAlive13);
	shard13->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/shards/shard13.obj");
	shard13->assign<Transform3D>(transform->position, scale);

	// Shard 14
	glm::vec3 randDir14 = getRandomDirection();
	float timeAlive14 = getRandomFloat() * 300.0f;
	Entity* shard14 = world->create();
	shard14->assign<TemporaryComponent>(randDir14, timeAlive14);
	shard14->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/shards/shard14.obj");
	shard14->assign<Transform3D>(transform->position, scale);

	// Shard 15
	glm::vec3 randDir15 = getRandomDirection();
	float timeAlive15 = getRandomFloat() * 300.0f;
	Entity* shard15 = world->create();
	shard15->assign<TemporaryComponent>(randDir15, timeAlive15);
	shard15->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/shards/shard15.obj");
	shard15->assign<Transform3D>(transform->position, scale);

	// Shard 16
	glm::vec3 randDir16 = getRandomDirection();
	float timeAlive16 = getRandomFloat() * 300.0f;
	Entity* shard16 = world->create();
	shard16->assign<TemporaryComponent>(randDir16, timeAlive16);
	shard16->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/shards/shard16.obj");
	shard16->assign<Transform3D>(transform->position, scale);

	// Shard 17
	glm::vec3 randDir17 = getRandomDirection();
	float timeAlive17 = getRandomFloat() * 300.0f;
	Entity* shard17 = world->create();
	shard17->assign<TemporaryComponent>(randDir17, timeAlive17);
	shard17->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/shards/shard17.obj");
	shard17->assign<Transform3D>(transform->position, scale);

	// Shard 18
	glm::vec3 randDir18 = getRandomDirection();
	float timeAlive18 = getRandomFloat() * 300.0f;
	Entity* shard18 = world->create();
	shard18->assign<TemporaryComponent>(randDir18, timeAlive18);
	shard18->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/shards/shard18.obj");
	shard18->assign<Transform3D>(transform->position, scale);

	// Shard 19
	glm::vec3 randDir19 = getRandomDirection();
	float timeAlive19 = getRandomFloat() * 300.0f;
	Entity* shard19 = world->create();
	shard19->assign<TemporaryComponent>(randDir19, timeAlive19);
	shard19->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/shards/shard19.obj");
	shard19->assign<Transform3D>(transform->position, scale);

	// Shard 20
	glm::vec3 randDir20 = getRandomDirection();
	float timeAlive20 = getRandomFloat() * 300.0f;
	Entity* shard20 = world->create();
	shard20->assign<TemporaryComponent>(randDir20, timeAlive20);
	shard20->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/shards/shard20.obj");
	shard20->assign<Transform3D>(transform->position, scale);

	// Shard 21
	glm::vec3 randDir21 = getRandomDirection();
	float timeAlive21 = getRandomFloat() * 300.0f;
	Entity* shard21 = world->create();
	shard21->assign<TemporaryComponent>(randDir21, timeAlive21);
	shard21->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/shards/shard21.obj");
	shard21->assign<Transform3D>(transform->position, scale);

	// Shard 22
	glm::vec3 randDir22 = getRandomDirection();
	float timeAlive22 = getRandomFloat() * 300.0f;
	Entity* shard22 = world->create();
	shard22->assign<TemporaryComponent>(randDir22, timeAlive22);
	shard22->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/shards/shard22.obj");
	shard22->assign<Transform3D>(transform->position, scale);

	// Shard 23
	glm::vec3 randDir23 = getRandomDirection();
	float timeAlive23 = getRandomFloat() * 300.0f;
	Entity* shard23 = world->create();
	shard23->assign<TemporaryComponent>(randDir23, timeAlive23);
	shard23->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/shards/shard23.obj");
	shard23->assign<Transform3D>(transform->position, scale);

	// Shard 24
	glm::vec3 randDir24 = getRandomDirection();
	float timeAlive24 = getRandomFloat() * 300.0f;
	Entity* shard24 = world->create();
	shard24->assign<TemporaryComponent>(randDir24, timeAlive24);
	shard24->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/shards/shard24.obj");
	shard24->assign<Transform3D>(transform->position, scale);

	// Shard 25
	glm::vec3 randDir25 = getRandomDirection();
	float timeAlive25 = getRandomFloat() * 300.0f;
	Entity* shard25 = world->create();
	shard25->assign<TemporaryComponent>(randDir25, timeAlive25);
	shard25->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/shards/shard25.obj");
	shard25->assign<Transform3D>(transform->position, scale);

}