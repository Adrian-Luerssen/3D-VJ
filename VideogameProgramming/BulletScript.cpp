#include "BulletScript.h"

void BulletScript::startScript() {

}

void BulletScript::tickScript(float deltaTime) {
	ComponentHandle<GameController> game;
	world->each<GameController>([&](Entity* ent, ComponentHandle<GameController> gameController) {game = gameController; });
	if (game->pause) return;

	world->each<BulletComponent>([&](Entity* ent, ComponentHandle<BulletComponent> bullet) {
		ComponentHandle<Transform3D> transform = ent->get<Transform3D>();
		float renderDistance = 5000;
		glm::vec3 userPos = glm::vec3(0);
		world->each<Camera>([&](Entity* ent, ComponentHandle<Camera> cam) {
			renderDistance = cam->renderDistance;
			userPos = cam->eye;
			});
		renderDistance += renderDistance / 10;
		glm::vec3 entityPos = ent->get<Transform3D>()->position;

		// Calculate the distance between the user and the entity
		float distance = glm::length(entityPos - userPos);

		if (distance > renderDistance /* && !ent->get<EnemyComponent>()->destroyed*/) {
			cout << "destroying bullet" << endl;
			//bullet->idle = true;
			ent->getWorld()->destroy(ent);
			return;
		}

		CheckCollisions(ent);

		//cout << "tick ball" << endl;
		//cout << "pos: " << transform->position.x << ", " << transform->position.y << endl;

		transform->position -= bullet->dir * deltaTime / this->speed;
		ComponentHandle<EnemyComponent> enemy = ent->get<EnemyComponent>();
		if (enemy != NULL) {
			if (enemy->destroyed) {
				world->destroy(ent);
			}
		}


		// Calculate the rotation angles needed to align the object with the camera direction

		});




}

void BulletScript::CheckCollisions(Entity* entity) {
	ComponentHandle<Transform3D> bulletTransform = entity->get<Transform3D>();
	ComponentHandle<CubeCollider> bulletCollider = entity->get<CubeCollider>();
	bool col = false;
	world->each<EnemyComponent>([&](Entity* ent, ComponentHandle<EnemyComponent> enemy) {
		if (col) return;
		if (entity->get<EnemyComponent>() != NULL && enemy->name == "Enemy Bullet") { /*cout << "HITTING SELF" << endl;*/ return; }
		ComponentHandle<Transform3D> enemyTransform = ent->get<Transform3D>();
		ComponentHandle<CubeCollider> enemyCollider = ent->get<CubeCollider>();
		if (enemyCollider == NULL) return;
		if (!entity->get<BulletComponent>()->userBullet && enemy->name == "Ship") return;
		// Check for collision along the X-axis
		bool collisionX = bulletTransform->position.x + bulletCollider->width >= enemyTransform->position.x - enemyCollider->width &&
			enemyTransform->position.x + enemyCollider->width >= bulletTransform->position.x - bulletCollider->width;

		// Check for collision along the Y-axis
		bool collisionY = bulletTransform->position.y + bulletCollider->height >= enemyTransform->position.y - enemyCollider->height &&
			enemyTransform->position.y + enemyCollider->height >= bulletTransform->position.y - bulletCollider->height;

		// Check for collision along the Z-axis
		bool collisionZ = bulletTransform->position.z + bulletCollider->length >= enemyTransform->position.z - enemyCollider->length &&
			enemyTransform->position.z + enemyCollider->length >= bulletTransform->position.z - bulletCollider->length;

		// If there is a collision along all axes, then a collision occurred
		if (collisionX && collisionY && collisionZ && !enemy->destroyed) {
			// Collision happened, you can handle it here
			cout << "COLLISION!!!!!!!!!!!" << endl;
			if (entity->get<BulletComponent>()->userBullet) {
				world->each<GameController>([&](Entity* ent, ComponentHandle<GameController> game) {game->score += enemy->points; });
			}
			enemy->destroyed = true;
			ent->getWorld()->destroy(entity);
			col = true;
		}
		});





}




