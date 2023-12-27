#include "SpawnerScript.h"

void SpawnerScript::startScript()
{
}

void SpawnerScript::tickScript(float deltaTime)
{
	ComponentHandle<GameController> game;
	world->each<GameController>([&](Entity* ent, ComponentHandle<GameController> gameController) {game = gameController; });
	if (game->pause) {
		firstSpawn = true;
		return;
	}
	else {

		glm::vec3 userPos = glm::vec3(0.0f);
		// spawn bullets on mouse click
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			if (!firstSpawn) {
				if (firstClick) {
					ComponentHandle<GameController> game;
					world->each<GameController>([&](Entity* ent, ComponentHandle<GameController> gameController) {game = gameController; });
					firstClick = false;
					t = 0;
					glm::vec3 objRot = glm::vec3(0.0f);
					glm::vec3 userLookAt = glm::vec3(0.0f);
					world->each<UserComponent>([&](Entity* ent, ComponentHandle<UserComponent> user) {
						userPos = ent->get<Transform3D>()->position;
						objRot = ent->get<Transform3D>()->rotation;

						glm::mat4 rotationMatrix =
							glm::rotate(glm::mat4(1.0f), objRot.y, glm::vec3(0.0f, 1.0f, 0.0f)) *
							glm::rotate(glm::mat4(1.0f), objRot.x, glm::vec3(1.0f, 0.0f, 0.0f)) *
							glm::rotate(glm::mat4(1.0f), objRot.z, glm::vec3(0.0f, 0.0f, 1.0f));

						glm::vec3 forward = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)));
						userLookAt = glm::normalize(forward);
						});

					//userLookAt = -userLookAt;
					// Assuming userPos is the ship's position, and userLookAt is the ship's facing direction

					// Calculate the offset distance for each side
					float offset = 3.0f;  // Adjust this value based on your requirements

					// Calculate the offset vectors for both sides
					glm::vec3 offsetLeft = glm::normalize(glm::cross(userLookAt, glm::vec3(0, 1, 0))) * offset;
					glm::vec3 offsetRight = -offsetLeft;  // Opposite direction for the right side

					// Calculate the spawn positions for both bullets
					glm::vec3 bulletSpawnPosLeft = userPos + offsetLeft;
					glm::vec3 bulletSpawnPosRight = userPos + offsetRight;

					cout << "Left bullet spawned at " << bulletSpawnPosLeft.x << "," << bulletSpawnPosLeft.y << "," << bulletSpawnPosLeft.z << endl;
					cout << "Right bullet spawned at " << bulletSpawnPosRight.x << "," << bulletSpawnPosRight.y << "," << bulletSpawnPosRight.z << endl;

					// Spawn left bullet
					Entity* entLeft = world->create();
					entLeft->assign<Transform3D>(bulletSpawnPosLeft, 5, objRot);
					entLeft->assign<MeshComponent>("Textures/flat_normal.png", "Meshes/bullet.obj", "bullet");
					entLeft->assign<BulletComponent>(bulletSpawnPosLeft, userLookAt);
					entLeft->assign<CubeCollider>(2, 2, 2);

					// Spawn right bullet
					Entity* entRight = world->create();
					entRight->assign<Transform3D>(bulletSpawnPosRight, 5, objRot);
					entRight->assign<MeshComponent>("Textures/flat_normal.png", "Meshes/bullet.obj", "bullet");
					entRight->assign<BulletComponent>(bulletSpawnPosRight, userLookAt);
					entRight->assign<CubeCollider>(2, 2, 2);
					float volume = 0.0f;
					//calculate volume based on distance to player
					world->each<Camera>([&](Entity* ent, ComponentHandle<Camera> camera) {
						float distance = glm::length(bulletSpawnPosRight - camera->eye);
						volume = 1.0f - (distance / camera->renderDistance);
						});
					entLeft->assign<SoundComponent>("Sounds/Lazer.wav", volume);
					entLeft->get<SoundComponent>()->playSound = true;
					entRight->assign<SoundComponent>("Sounds/Lazer.wav", volume);
					entRight->get<SoundComponent>()->playSound = true;

					game->imunity = false;
				}
			}

		}
		else {
			firstClick = true;
			if (firstSpawn) {
				firstSpawn = false;
			}
		}
		t += deltaTime / 2.0f;
		t2 += deltaTime / 2.0f;
		if (t > delay) {
			Entity* ent = world->create();
			ent->assign<Transform3D>(glm::vec3(1000000, 1, 0.), 5);
			int random = rand() % 10 + 1;
			cout << "spawning new asteroid type " << random << endl;
			switch (random) {
			case 1:
				ent->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/asteroide1.obj", "default", "Textures/asteroids/normal.png");
				break;
			case 2:
				ent->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/asteroide2.obj", "default", "Textures/asteroids/normal.png");
				break;
			case 3:
				ent->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/asteroide3.obj", "default", "Textures/asteroids/normal.png");
				break;
			case 4:
				ent->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/asteroide4.obj", "default", "Textures/asteroids/normal.png");
				break;
			case 5:
				ent->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/asteroide5.obj", "default", "Textures/asteroids/normal.png");
				break;
			case 6:
				ent->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/asteroide6.obj", "default", "Textures/asteroids/normal.png");
				break;
			case 7:
				ent->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/asteroide7.obj", "default", "Textures/asteroids/normal.png");
				break;
			case 8:
				ent->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/asteroide8.obj", "default", "Textures/asteroids/normal.png");
				break;
			case 9:
				ent->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/asteroide9.obj", "default", "Textures/asteroids/normal.png");
				break;
			case 10:
				ent->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/asteroide10.obj", "default", "Textures/asteroids/normal.png");
				break;
			}
			ent->assign<EnemyComponent>(1, counter, "Asteroid");
			ent->assign<CubeCollider>(25, 25, 25);/**/
			counter++;
			t = 0;
		}

		if (t2 > shipDelay) {
			t2 = 0;
			Entity* enemy = world->create();
			enemy->assign<Transform3D>(glm::vec3(1000000, 1, 0.), 1);
			enemy->assign<MeshComponent>("Textures/Enemy/spaceship_baseColor.png", "Meshes/Enemy/enemy.obj", "user", "Textures/Enemy/spaceship_normal.png");
			enemy->get<MeshComponent>()->roughnessFilepath = "Textures/Enemy/spaceship_roughness.png";
			//enemy->get<MeshComponent>()->metallicFilepath = "Textures/Enemy/spaceship_metallic.png";
			enemy->get<MeshComponent>()->emissiveFilepath = "Textures/Enemy/spaceship_emit.png";
			enemy->assign<EnemyComponent>(5, counter, "Ship");
			enemy->assign<CubeCollider>(50, 50, 50);

			Entity* canon = world->create();
			canon->assign<Transform3D>(glm::vec3(1000000, 1, 0.), 1);
			canon->assign<MeshComponent>("Textures/Enemy/cannon_diffuse2.png", "Meshes/Enemy/canon.obj", "user", "Textures/Enemy/cannon_normal.png");
			canon->get<MeshComponent>()->roughnessFilepath = "Textures/Enemy/cannon_roughness.png";
			//canon->get<MeshComponent>()->metallicFilepath = "Textures/Enemy/cannon_metallic.png";
			canon->get<MeshComponent>()->emissiveFilepath = "Textures/Enemy/cannon_emit.png";
			canon->get<MeshComponent>()->shaderName = "user";
			canon->assign<EnemyShipCanon>(glm::vec3(0), glm::vec3(0), counter);
			counter++;

		}
		world->each<EnemyComponent>([&](Entity* shipEntity, ComponentHandle<EnemyComponent> enemyComp) {
			if (enemyComp->name != "Ship") return;
			int shipID = enemyComp->id;

			
			world->each<EnemyShipCanon>([&](Entity* ent, ComponentHandle<EnemyShipCanon> enemyCanon) {
				// Calculate direction vector from enemy cannon to user position
				if (enemyCanon->ticks > enemyCanon->ticksToFire) {
					glm::vec3 canonPos = ent->get<Transform3D>()->position;
					glm::vec3 cannonRot = ent->get<Transform3D>()->rotation;

					glm::mat4 rotationMatrix =
						glm::rotate(glm::mat4(1.0f), cannonRot.y, glm::vec3(0.0f, 1.0f, 0.0f)) *
						glm::rotate(glm::mat4(1.0f), cannonRot.x, glm::vec3(1.0f, 0.0f, 0.0f)) *
						glm::rotate(glm::mat4(1.0f), cannonRot.z, glm::vec3(0.0f, 0.0f, 1.0f));

					glm::vec3 forward = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)));
					glm::vec3 cannonLookAt = glm::normalize(forward);
					Entity* entLeft = world->create();
					entLeft->assign<Transform3D>(canonPos, 10, cannonRot);
					entLeft->assign<MeshComponent>("Textures/flat_normal.png", "Meshes/bullet.obj", "bullet");
					entLeft->assign<BulletComponent>(canonPos, -cannonLookAt,false);
					entLeft->assign<EnemyComponent>(0, -1, "Enemy Bullet");
					entLeft->assign<CubeCollider>(2, 2, 2);
					enemyCanon->ticksToFire = std::rand() % 300;
					enemyCanon->ticks = 0;
					float volume = 0.0f;
					//calculate volume based on distance to player
					world->each<Camera>([&](Entity* ent, ComponentHandle<Camera> camera) {
						float distance = glm::length(canonPos - camera->eye);
						volume = 1.0f - (distance / camera->renderDistance);
						});
					entLeft->assign<SoundComponent>("Sounds/EnemyLazer.wav", volume);
					entLeft->get<SoundComponent>()->playSound = true;
					//cout << "Enemy Cannon fired" << endl;
				}
				else {
					enemyCanon->ticks += deltaTime / 10.0f;
					//cout << "Ticking the canon " << enemyCanon->ticks << endl;
				}

				});

			});

	}


}
