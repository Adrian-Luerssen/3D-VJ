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
		if (!enemyComp->spawned) {
			generateStartPos(1, renderDistance, userPos);
			enemyComp->startPos = startPos;
			enemyComp->dir = dir;
			shipEntity->get<Transform3D>()->position = startPos;

			// Calculate rotation matrix using lookAt
			glm::mat4 rotationMatrix = glm::lookAt(glm::vec3(0.0f), enemyComp->dir, glm::vec3(0.0f, 1.0f, 0.0f));

			// Extract pitch and yaw from the rotation matrix
			float pitch = asin(-rotationMatrix[1][2]);
			float yaw = atan2(rotationMatrix[0][2], rotationMatrix[2][2]);

			shipEntity->get<Transform3D>()->rotation = -glm::vec3(pitch, yaw, 0.0f);

			enemyComp->spawned = true;
		}
		int shipID = enemyComp->id;
		world->each<EnemyShipCanon>([&](Entity* entity, ComponentHandle<EnemyShipCanon> enemyCanon) {
			// Calculate direction vector from cannon to user position
			if (enemyCanon->shipID != shipID) return;
			if (enemyComp->destroyed) {
				world->destroy(shipEntity);
				world->destroy(entity);
				spawnShards(shipEntity->get<Transform3D>());
				return;
			}
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
		shipEntity->get<Transform3D>()->position += enemyComp->dir;
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

	// Only randomize the x-component of the direction vector
	float randomnessFactor = 0.4f; // You can adjust this value based on how much randomness you want
	dir.x += (static_cast<float>(std::rand()) / RAND_MAX - 0.5f) * randomnessFactor;

	// Set y and z components to zero
	dir.y = 0.0f;
	dir.z = 0.0f;

	// Normalize the direction vector again to maintain unit length
	dir = glm::normalize(dir);

	// Set the member variables
	this->startPos = startPos;
	this->dir = dir;

	this->ticksBeforeAppear = rand() % (10000 / level);
}


float EnemyShipScript::getRandomFloat() {
	return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

// Function to generate a random direction vector
glm::vec3 EnemyShipScript::getRandomDirection() {
	return glm::normalize(glm::vec3(static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2.0f - 1.0f, static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2.0f - 1.0f, static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2.0f - 1.0f));
}
void EnemyShipScript::spawnShards(ComponentHandle<Transform3D> transform) {
	cout << "SHIP DESTROYED" << endl;
	float scale = 1.0f;

	Entity* shard1 = world->create();
	shard1->assign<TemporaryComponent>(getRandomDirection(), getRandomFloat() * 300.0f);
	shard1->assign<MeshComponent>("Textures/Enemy/spaceship_baseColor.png", "Meshes/Enemy/shard1.obj", "user", "Textures/Enemy/spaceship_normal.png");
	shard1->get<MeshComponent>()->roughnessFilepath = "Textures/Enemy/spaceship_roughness2.png";
	//shard1->get<MeshComponent>()->metallicFilepath = "Textures/Enemy/spaceship_metallic.png";
	shard1->get<MeshComponent>()->emissiveFilepath = "Textures/Enemy/spaceship_emit.png";
	shard1->assign<Transform3D>(transform->position, scale);

	float volume = 0.0f;
	world->each<Camera>([&](Entity* ent, ComponentHandle<Camera> camera) {
		float distance = glm::length(transform->position - camera->eye);
		volume = 1.0f - (distance / camera->renderDistance);
		});

	shard1->assign<SoundComponent>("Sounds/Explosion.wav", volume);
	shard1->get<SoundComponent>()->playSound = true;

	Entity* shard2 = world->create();
	shard2->assign<TemporaryComponent>(getRandomDirection(), getRandomFloat() * 300.0f);
	shard2->assign<MeshComponent>("Textures/Enemy/spaceship_baseColor.png", "Meshes/Enemy/shard2.obj", "user", "Textures/Enemy/spaceship_normal.png");
	shard2->get<MeshComponent>()->roughnessFilepath = "Textures/Enemy/spaceship_roughness2.png";
	//shard2->get<MeshComponent>()->metallicFilepath = "Textures/Enemy/spaceship_metallic.png";
	shard2->get<MeshComponent>()->emissiveFilepath = "Textures/Enemy/spaceship_emit.png";
	shard2->assign<Transform3D>(transform->position, scale);

	Entity* shard3 = world->create();
	shard3->assign<TemporaryComponent>(getRandomDirection(), getRandomFloat() * 300.0f);
	shard3->assign<MeshComponent>("Textures/Enemy/spaceship_baseColor.png", "Meshes/Enemy/shard3.obj", "user", "Textures/Enemy/spaceship_normal.png");
	shard3->get<MeshComponent>()->roughnessFilepath = "Textures/Enemy/spaceship_roughness2.png";
	//shard3->get<MeshComponent>()->metallicFilepath = "Textures/Enemy/spaceship_metallic.png";
	shard3->get<MeshComponent>()->emissiveFilepath = "Textures/Enemy/spaceship_emit.png";
	shard3->assign<Transform3D>(transform->position, scale);

	Entity* shard4 = world->create();
	shard4->assign<TemporaryComponent>(getRandomDirection(), getRandomFloat() * 300.0f);
	shard4->assign<MeshComponent>("Textures/Enemy/spaceship_baseColor.png", "Meshes/Enemy/shard4.obj", "user", "Textures/Enemy/spaceship_normal.png");
	shard4->get<MeshComponent>()->roughnessFilepath = "Textures/Enemy/spaceship_roughness2.png";
	// shard4->get<MeshComponent>()->metallicFilepath = "Textures/Enemy/spaceship_metallic.png";
	shard4->get<MeshComponent>()->emissiveFilepath = "Textures/Enemy/spaceship_emit.png";
	shard4->assign<Transform3D>(transform->position, scale);

	Entity* shard5 = world->create();
	shard5->assign<TemporaryComponent>(getRandomDirection(), getRandomFloat() * 300.0f);
	shard5->assign<MeshComponent>("Textures/Enemy/spaceship_baseColor.png", "Meshes/Enemy/shard5.obj", "user", "Textures/Enemy/spaceship_normal.png");
	shard5->get<MeshComponent>()->roughnessFilepath = "Textures/Enemy/spaceship_roughness2.png";
	// shard5->get<MeshComponent>()->metallicFilepath = "Textures/Enemy/spaceship_metallic.png";
	shard5->get<MeshComponent>()->emissiveFilepath = "Textures/Enemy/spaceship_emit.png";
	shard5->assign<Transform3D>(transform->position, scale);

	Entity* shard6 = world->create();
	shard6->assign<TemporaryComponent>(getRandomDirection(), getRandomFloat() * 300.0f);
	shard6->assign<MeshComponent>("Textures/Enemy/spaceship_baseColor.png", "Meshes/Enemy/shard6.obj", "user", "Textures/Enemy/spaceship_normal.png");
	shard6->get<MeshComponent>()->roughnessFilepath = "Textures/Enemy/spaceship_roughness2.png";
	// shard6->get<MeshComponent>()->metallicFilepath = "Textures/Enemy/spaceship_metallic.png";
	shard6->get<MeshComponent>()->emissiveFilepath = "Textures/Enemy/spaceship_emit.png";
	shard6->assign<Transform3D>(transform->position, scale);

	Entity* shard7 = world->create();
	shard7->assign<TemporaryComponent>(getRandomDirection(), getRandomFloat() * 300.0f);
	shard7->assign<MeshComponent>("Textures/Enemy/spaceship_baseColor.png", "Meshes/Enemy/shard7.obj", "user", "Textures/Enemy/spaceship_normal.png");
	shard7->get<MeshComponent>()->roughnessFilepath = "Textures/Enemy/spaceship_roughness2.png";
	// shard7->get<MeshComponent>()->metallicFilepath = "Textures/Enemy/spaceship_metallic.png";
	shard7->get<MeshComponent>()->emissiveFilepath = "Textures/Enemy/spaceship_emit.png";
	shard7->assign<Transform3D>(transform->position, scale);

	Entity* shard8 = world->create();
	shard8->assign<TemporaryComponent>(getRandomDirection(), getRandomFloat() * 300.0f);
	shard8->assign<MeshComponent>("Textures/Enemy/spaceship_baseColor.png", "Meshes/Enemy/shard8.obj", "user", "Textures/Enemy/spaceship_normal.png");
	shard8->get<MeshComponent>()->roughnessFilepath = "Textures/Enemy/spaceship_roughness2.png";
	// shard8->get<MeshComponent>()->metallicFilepath = "Textures/Enemy/spaceship_metallic.png";
	shard8->get<MeshComponent>()->emissiveFilepath = "Textures/Enemy/spaceship_emit.png";
	shard8->assign<Transform3D>(transform->position, scale);

	Entity* shard9 = world->create();
	shard9->assign<TemporaryComponent>(getRandomDirection(), getRandomFloat() * 300.0f);
	shard9->assign<MeshComponent>("Textures/Enemy/spaceship_baseColor.png", "Meshes/Enemy/shard9.obj", "user", "Textures/Enemy/spaceship_normal.png");
	shard9->get<MeshComponent>()->roughnessFilepath = "Textures/Enemy/spaceship_roughness2.png";
	// shard9->get<MeshComponent>()->metallicFilepath = "Textures/Enemy/spaceship_metallic.png";
	shard9->get<MeshComponent>()->emissiveFilepath = "Textures/Enemy/spaceship_emit.png";
	shard9->assign<Transform3D>(transform->position, scale);

	Entity* shard10 = world->create();
	shard10->assign<TemporaryComponent>(getRandomDirection(), getRandomFloat() * 300.0f);
	shard10->assign<MeshComponent>("Textures/Enemy/spaceship_baseColor.png", "Meshes/Enemy/shard10.obj", "user", "Textures/Enemy/spaceship_normal.png");
	shard10->get<MeshComponent>()->roughnessFilepath = "Textures/Enemy/spaceship_roughness2.png";
	// shard10->get<MeshComponent>()->metallicFilepath = "Textures/Enemy/spaceship_metallic.png";
	shard10->get<MeshComponent>()->emissiveFilepath = "Textures/Enemy/spaceship_emit.png";
	shard10->assign<Transform3D>(transform->position, scale);

	Entity* shard11 = world->create();
	shard11->assign<TemporaryComponent>(getRandomDirection(), getRandomFloat() * 300.0f);
	shard11->assign<MeshComponent>("Textures/Enemy/spaceship_baseColor.png", "Meshes/Enemy/shard11.obj", "user", "Textures/Enemy/spaceship_normal.png");
	shard11->get<MeshComponent>()->roughnessFilepath = "Textures/Enemy/spaceship_roughness2.png";
	// shard11->get<MeshComponent>()->metallicFilepath = "Textures/Enemy/spaceship_metallic.png";
	shard11->get<MeshComponent>()->emissiveFilepath = "Textures/Enemy/spaceship_emit.png";
	shard11->assign<Transform3D>(transform->position, scale);

	Entity* shard12 = world->create();
	shard12->assign<TemporaryComponent>(getRandomDirection(), getRandomFloat() * 300.0f);
	shard12->assign<MeshComponent>("Textures/Enemy/spaceship_baseColor.png", "Meshes/Enemy/shard12.obj", "user", "Textures/Enemy/spaceship_normal.png");
	shard12->get<MeshComponent>()->roughnessFilepath = "Textures/Enemy/spaceship_roughness2.png";
	// shard12->get<MeshComponent>()->metallicFilepath = "Textures/Enemy/spaceship_metallic.png";
	shard12->get<MeshComponent>()->emissiveFilepath = "Textures/Enemy/spaceship_emit.png";
	shard12->assign<Transform3D>(transform->position, scale);

	Entity* shard13 = world->create();
	shard13->assign<TemporaryComponent>(getRandomDirection(), getRandomFloat() * 300.0f);
	shard13->assign<MeshComponent>("Textures/Enemy/spaceship_baseColor.png", "Meshes/Enemy/shard13.obj", "user", "Textures/Enemy/spaceship_normal.png");
	shard13->get<MeshComponent>()->roughnessFilepath = "Textures/Enemy/spaceship_roughness2.png";
	// shard13->get<MeshComponent>()->metallicFilepath = "Textures/Enemy/spaceship_metallic.png";
	shard13->get<MeshComponent>()->emissiveFilepath = "Textures/Enemy/spaceship_emit.png";
	shard13->assign<Transform3D>(transform->position, scale);

	Entity* shard14 = world->create();
	shard14->assign<TemporaryComponent>(getRandomDirection(), getRandomFloat() * 300.0f);
	shard14->assign<MeshComponent>("Textures/Enemy/spaceship_baseColor.png", "Meshes/Enemy/shard14.obj", "user", "Textures/Enemy/spaceship_normal.png");
	shard14->get<MeshComponent>()->roughnessFilepath = "Textures/Enemy/spaceship_roughness2.png";
	// shard14->get<MeshComponent>()->metallicFilepath = "Textures/Enemy/spaceship_metallic.png";
	shard14->get<MeshComponent>()->emissiveFilepath = "Textures/Enemy/spaceship_emit.png";
	shard14->assign<Transform3D>(transform->position, scale);

	Entity* shard15 = world->create();
	shard15->assign<TemporaryComponent>(getRandomDirection(), getRandomFloat() * 300.0f);
	shard15->assign<MeshComponent>("Textures/Enemy/spaceship_baseColor.png", "Meshes/Enemy/shard15.obj", "user", "Textures/Enemy/spaceship_normal.png");
	shard15->get<MeshComponent>()->roughnessFilepath = "Textures/Enemy/spaceship_roughness2.png";
	// shard15->get<MeshComponent>()->metallicFilepath = "Textures/Enemy/spaceship_metallic.png";
	shard15->get<MeshComponent>()->emissiveFilepath = "Textures/Enemy/spaceship_emit.png";
	shard15->assign<Transform3D>(transform->position, scale);

	Entity* shard16 = world->create();
	shard16->assign<TemporaryComponent>(getRandomDirection(), getRandomFloat() * 300.0f);
	shard16->assign<MeshComponent>("Textures/Enemy/spaceship_baseColor.png", "Meshes/Enemy/shard16.obj", "user", "Textures/Enemy/spaceship_normal.png");
	shard16->get<MeshComponent>()->roughnessFilepath = "Textures/Enemy/spaceship_roughness2.png";
	// shard16->get<MeshComponent>()->metallicFilepath = "Textures/Enemy/spaceship_metallic.png";
	shard16->get<MeshComponent>()->emissiveFilepath = "Textures/Enemy/spaceship_emit.png";
	shard16->assign<Transform3D>(transform->position, scale);

	Entity* shard17 = world->create();
	shard17->assign<TemporaryComponent>(getRandomDirection(), getRandomFloat() * 300.0f);
	shard17->assign<MeshComponent>("Textures/Enemy/spaceship_baseColor.png", "Meshes/Enemy/shard17.obj", "user", "Textures/Enemy/spaceship_normal.png");
	shard17->get<MeshComponent>()->roughnessFilepath = "Textures/Enemy/spaceship_roughness2.png";
	// shard17->get<MeshComponent>()->metallicFilepath = "Textures/Enemy/spaceship_metallic.png";
	shard17->get<MeshComponent>()->emissiveFilepath = "Textures/Enemy/spaceship_emit.png";
	shard17->assign<Transform3D>(transform->position, scale);

	Entity* shard18 = world->create();
	shard18->assign<TemporaryComponent>(getRandomDirection(), getRandomFloat() * 300.0f);
	shard18->assign<MeshComponent>("Textures/Enemy/spaceship_baseColor.png", "Meshes/Enemy/shard18.obj", "user", "Textures/Enemy/spaceship_normal.png");
	shard18->get<MeshComponent>()->roughnessFilepath = "Textures/Enemy/spaceship_roughness2.png";
	// shard18->get<MeshComponent>()->metallicFilepath = "Textures/Enemy/spaceship_metallic.png";
	shard18->get<MeshComponent>()->emissiveFilepath = "Textures/Enemy/spaceship_emit.png";
	shard18->assign<Transform3D>(transform->position, scale);

	Entity* shard19 = world->create();
	shard19->assign<TemporaryComponent>(getRandomDirection(), getRandomFloat() * 300.0f);
	shard19->assign<MeshComponent>("Textures/Enemy/spaceship_baseColor.png", "Meshes/Enemy/shard19.obj", "user", "Textures/Enemy/spaceship_normal.png");
	shard19->get<MeshComponent>()->roughnessFilepath = "Textures/Enemy/spaceship_roughness2.png";
	// shard19->get<MeshComponent>()->metallicFilepath = "Textures/Enemy/spaceship_metallic.png";
	shard19->get<MeshComponent>()->emissiveFilepath = "Textures/Enemy/spaceship_emit.png";
	shard19->assign<Transform3D>(transform->position, scale);

	Entity* shard20 = world->create();
	shard20->assign<TemporaryComponent>(getRandomDirection(), getRandomFloat() * 300.0f);
	shard20->assign<MeshComponent>("Textures/Enemy/spaceship_baseColor.png", "Meshes/Enemy/shard20.obj", "user", "Textures/Enemy/spaceship_normal.png");
	shard20->get<MeshComponent>()->roughnessFilepath = "Textures/Enemy/spaceship_roughness2.png";
	// shard20->get<MeshComponent>()->metallicFilepath = "Textures/Enemy/spaceship_metallic.png";
	shard20->get<MeshComponent>()->emissiveFilepath = "Textures/Enemy/spaceship_emit.png";
	shard20->assign<Transform3D>(transform->position, scale);

	Entity* shard21 = world->create();
	shard21->assign<TemporaryComponent>(getRandomDirection(), getRandomFloat() * 300.0f);
	shard21->assign<MeshComponent>("Textures/Enemy/spaceship_baseColor.png", "Meshes/Enemy/shard21.obj", "user", "Textures/Enemy/spaceship_normal.png");
	shard21->get<MeshComponent>()->roughnessFilepath = "Textures/Enemy/spaceship_roughness2.png";
	// shard21->get<MeshComponent>()->metallicFilepath = "Textures/Enemy/spaceship_metallic.png";
	shard21->get<MeshComponent>()->emissiveFilepath = "Textures/Enemy/spaceship_emit.png";
	shard21->assign<Transform3D>(transform->position, scale);

	Entity* shard22 = world->create();
	shard22->assign<TemporaryComponent>(getRandomDirection(), getRandomFloat() * 300.0f);
	shard22->assign<MeshComponent>("Textures/Enemy/spaceship_baseColor.png", "Meshes/Enemy/shard22.obj", "user", "Textures/Enemy/spaceship_normal.png");
	shard22->get<MeshComponent>()->roughnessFilepath = "Textures/Enemy/spaceship_roughness2.png";
	// shard22->get<MeshComponent>()->metallicFilepath = "Textures/Enemy/spaceship_metallic.png";
	shard22->get<MeshComponent>()->emissiveFilepath = "Textures/Enemy/spaceship_emit.png";
	shard22->assign<Transform3D>(transform->position, scale);

	Entity* shard23 = world->create();
	shard23->assign<TemporaryComponent>(getRandomDirection(), getRandomFloat() * 300.0f);
	shard23->assign<MeshComponent>("Textures/Enemy/spaceship_baseColor.png", "Meshes/Enemy/shard23.obj", "user", "Textures/Enemy/spaceship_normal.png");
	shard23->get<MeshComponent>()->roughnessFilepath = "Textures/Enemy/spaceship_roughness2.png";
	// shard23->get<MeshComponent>()->metallicFilepath = "Textures/Enemy/spaceship_metallic.png";
	shard23->get<MeshComponent>()->emissiveFilepath = "Textures/Enemy/spaceship_emit.png";
	shard23->assign<Transform3D>(transform->position, scale);

	Entity* shard24 = world->create();
	shard24->assign<TemporaryComponent>(getRandomDirection(), getRandomFloat() * 300.0f);
	shard24->assign<MeshComponent>("Textures/Enemy/spaceship_baseColor.png", "Meshes/Enemy/shard24.obj", "user", "Textures/Enemy/spaceship_normal.png");
	shard24->get<MeshComponent>()->roughnessFilepath = "Textures/Enemy/spaceship_roughness2.png";
	// shard24->get<MeshComponent>()->metallicFilepath = "Textures/Enemy/spaceship_metallic.png";
	shard24->get<MeshComponent>()->emissiveFilepath = "Textures/Enemy/spaceship_emit.png";
	shard24->assign<Transform3D>(transform->position, scale);


}