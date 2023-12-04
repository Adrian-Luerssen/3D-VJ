#include "SpawnerScript.h"

void SpawnerScript::startScript()
{
}

void SpawnerScript::tickScript(float deltaTime)
{

	
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
		

		//Entity* ent2d = world->create();
		//ent2d->assign<Transform2D>(glm::vec2(100. + counter*100, 100.), 0.f, 1.f);
		//ent2d->assign<Sprite>("Textures/science_dog.png", glm::vec3(1., 1., 1.), false, glm::vec2(100., 100.), "unlit");

		//counter += 1;
	
}
