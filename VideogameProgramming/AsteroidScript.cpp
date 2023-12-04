#include "AsteroidScript.h"
#include <cmath>




void AsteroidScript::tickScript(float deltaTime) {
	int currLevel = 2;
	
	//cout << "currLvl: " << currLevel << " disp leve: " << entity->get<EnemyComponent>()->level <<  endl;
	if (entity->get<EnemyComponent>()->level > currLevel) {
		//cout << "leveled up id = " << entity->get<EnemyComponent>()->id << endl;
		return;
	}
	//cout << "still here id = " << entity->get<EnemyComponent>()->id << endl;
	ComponentHandle<Transform3D> transform = entity->get<Transform3D>();

	if (entity->get<EnemyComponent>()->destroyed && hp <= 0) {
		if (ticksSinceDestroyed < 60) {
			ticksSinceDestroyed += deltaTime / 5.5f;
			
		}
		else {
			ticksSinceDestroyed = 0;
			//cout << "wtf is it here id = " << entity->get<EnemyComponent>()->id << endl;
			return;
		}
	} 
	if (this->ticksBeforeAppear > 0) {
		//cout << "waiting to appear id = "<< entity->get<EnemyComponent>()->id << endl;
		this->ticksBeforeAppear += -deltaTime/2.f;
		this->ticksBeforeAppear = this->ticksBeforeAppear < 0 ? 0 : this->ticksBeforeAppear;
		return;
	}
	else if (this->ticksBeforeAppear == 0) {
		this->ticksBeforeAppear--;
		transform->position = this->startPos;
		//cout << "asteroid appeared" << endl;
		//cout << "POS: " << transform->position.x << ", " << transform->position.y << ", " << transform->position.z << endl;
		//cout << "DIR: " << this->dir.x << ", " << this->dir.y << ", " << this->dir.z << endl;
		return;
	}
	float renderDistance = 5000;
	glm::vec3 userPos = glm::vec3(0);
	world->each<UserComponent>([&](Entity* ent, ComponentHandle<UserComponent> userComp) {
		renderDistance = userComp->renderDistance; 
		userPos = ent->get<Camera>()->position; 
		});
	renderDistance += renderDistance / 10;
	glm::vec3 entityPos = entity->get<Transform3D>()->position;

	// Calculate the distance between the user and the entity
	float distance = glm::length(entityPos - userPos);

	if (distance > renderDistance && !entity->get<EnemyComponent>()->destroyed) {
		//std::cout << "Out of bounds = " << entity->get<EnemyComponent>()->id << std::endl;

		// If it goes out of bounds, you can perform additional actions, e.g., reuse it.
		generateStartPos(currLevel,renderDistance,userPos);
	}


	ComponentHandle<CubeCollider> collider = entity->get<CubeCollider>();
	ComponentHandle<EnemyComponent> enemy = entity->get<EnemyComponent>();
	
	transform->position += (this->dir * deltaTime / 10.f);
	//transform->rotation += deltaTime / 3.f;
	if (!(!entity->get<EnemyComponent>()->destroyed && hp <= 0)) {
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

void AsteroidScript::generateStartPos(int level, float renderDistance, glm::vec3& userPos) {
	int face = std::rand() % 6; // Select a random face of the cube
	int other1 = (std::rand() % 2) ? 1 : -1;
	int other2 = (std::rand() % 2) ? 1 : -1;
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

	// Set the member variables
	this->startPos = startPos;
	this->dir = dir;

	// cout << "startPos: " << this->startPos.x << ", " << this->startPos.y << ", " << this->startPos.z << endl;
	// cout << "direction: " << this->dir.x << ", " << this->dir.y << ", " << this->dir.z << endl;
	this->ticksBeforeAppear = rand() % (10000 / level);
}

