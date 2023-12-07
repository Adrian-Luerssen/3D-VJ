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
			userPos = cam->position;
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

		transform->position -= bullet->dir *deltaTime / 3.0f;
		});
	

	
}

void BulletScript::CheckCollisions(Entity* entity) {
	ComponentHandle<Transform3D> bulletTransform = entity->get<Transform3D>();
	ComponentHandle<CubeCollider> bulletCollider = entity->get<CubeCollider>();
	bool col = false;
	world->each<EnemyComponent>([&](Entity* ent, ComponentHandle<EnemyComponent> enemy) {
		if (col) return;
		ComponentHandle<Transform3D> enemyTransform = ent->get<Transform3D>();
		ComponentHandle<CubeCollider> enemyCollider = ent->get<CubeCollider>();

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
		if (collisionX && collisionY && collisionZ) {
			// Collision happened, you can handle it here
			cout << "COLLISION!!!!!!!!!!!" << endl;
			ent->get<EnemyComponent>()->destroyed = true;
			ent->getWorld()->destroy(entity);
			col = true;
		}
		});



	

}



/*
void EnemyBulletScript::startScript() {

}

void EnemyBulletScript::tickScript(float deltaTime) {
	ComponentHandle<BoxCollider> collider = entity->get<BoxCollider>();
	ComponentHandle<BulletComponent> bullet = entity->get<BulletComponent>();
	ComponentHandle<Transform> transform = entity->get<Transform>();
	ComponentHandle<Sprite> sprite = entity->get<Sprite>();
	ComponentHandle<EnemyComponent> enemy = entity->get<EnemyComponent>();

	if (collider->collidedWith) {
		bullet->idle;
		transform->position.x = BulletScript::idlePos;
		transform->position.y = BulletScript::idlePos;
		collider->collidedWith = false;
	}
	
	if (bullet->shot) {
		entity->get<SoundComponent>()->playSound = true;
		ComponentHandle<BulletComponent> bullet = entity->get<BulletComponent>();
		//if (bullet->idle) return;
		ComponentHandle<Transform> transform = entity->get<Transform>();

		transform->position.x = bullet->posX;
		transform->position.y = bullet->posY;
		bullet->idle = false;
		bullet->shot = false;
		bullet->idle = false;
		ticksSinceFired = 0;
		sprite->filepath = "Textures/Bullet/Bullet_1.png";

		return;
	}
	if (bullet->idle) return;
	//cout << "bullet pos: " << transform->position.x << ", " << transform->position.y << endl;
	if ((transform->position.x > 800 || transform->position.x < 0) || (transform->position.y > 800 || transform->position.y < 0)) {
		cout << "Going idle" << endl;
		bullet->idle = true;
		transform->position.x = BulletScript::idlePos;
		transform->position.y = BulletScript::idlePos;
		return;
	}

	
		ticksSinceFired += deltaTime / 5.5f;
		if (ticksSinceFired >= 0 && ticksSinceFired < 10) {
			sprite->filepath = bullet->frame1;
		}
		else {
			int tickMod = static_cast<int>(round(ticksSinceFired)) % 30;

			if (tickMod >= 0 && tickMod < 10) {
				//cout << "sprite 1" << endl;
				sprite->filepath = bullet->frame2;
			}
			else if (tickMod >= 10 && tickMod < 20) {
				//cout << "sprite 2" << endl;
				sprite->filepath = bullet->frame3;
			}
			else {
				//cout << "sprite 3" << endl;
				sprite->filepath = bullet->frame4;
			}
		}

	

	// cout << "tick ball" << endl;
	// cout << "pos: " << transform->position.x << ", " << transform->position.y << endl;
	// cout << "tick enemy bullet" << endl;
	transform->position.y += bullet->dirY * deltaTime / 3.f;
	transform->position.x += bullet->dirX * deltaTime / 3.f;


}

*/