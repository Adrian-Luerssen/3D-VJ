#include "SpawnerScript.h"

void SpawnerScript::startScript()
{
}

void SpawnerScript::tickScript(float deltaTime)
{

		// spawn bullets on mouse click
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			if (firstClick) {
				firstClick = false;
				t = 0;
				glm::vec3 userPos = glm::vec3(0.0f);
				glm::vec3 userLookAt = glm::vec3(0.0f);
				world->each<UserComponent>([&](Entity* ent, ComponentHandle<UserComponent> userComp) {
					userPos = ent->get<Camera>()->position;
					userLookAt = ent->get<Camera>()->orientation;
					});
				userLookAt = glm::normalize(userLookAt);
				userLookAt = -userLookAt;
				cout << "spawned at "<<userPos.x<<","<<userPos.y<<","<<userPos.z << endl;
				Entity* ent = world->create();
				ent->assign<Transform3D>(userPos, 5);
				ent->assign<MeshComponent>("Textures/flat_normal.png","Meshes/cube.obj");
				ent->assign<BulletComponent>(userPos, userLookAt);
				ent->assign<CubeCollider>(2, 2, 2);
			}
			
		}
		else  {
			firstClick = true;
		}
		t += deltaTime / 2.0f;
		if (t > delay) {
			cout << "spawning new asteroid" << endl;
			Entity* ent = world->create();
			ent->assign<Transform3D>(glm::vec3(1000000, 1, 0.), 5);
			ent->assign<MeshComponent>("Textures/asteroids/color.png", "Meshes/asteroids/asteroide1.obj" , "default", "Textures/asteroids/normal.png");
			ent->assign<EnemyComponent>(1, counter);
			ent->assign<CubeCollider>(25, 25, 25);
			counter++;
			t = 0;
		}
		
	
}
