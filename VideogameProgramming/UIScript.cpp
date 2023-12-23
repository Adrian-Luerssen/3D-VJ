#include "UIScript.h"
#include <chrono>

void UIScript::startScript() {

}

void UIScript::tickScript(float deltaTime) {

	ComponentHandle<GameController> game;
	world->each<GameController>([&](Entity* ent, ComponentHandle<GameController> gameController) {game = gameController; });

	if (changePage) {
		
		if (page == "Menu") {
			game->pause = true;
			//destroy all UI elements
			world->each<UIComponent>([&](Entity* ent, ComponentHandle<UIComponent> uiComp) {
				world->destroy(ent);
				});
			Entity* ent = world->create();
			ent->assign<Transform2D>(glm::vec2(400., 400.), 0.0f, 1.0f);
			ent->assign<Sprite>("Textures/text/pulpoid.png", glm::vec3(1., 1., 1.));
			ent->assign<UIComponent>("Title", "Menu");

			Entity* ent2 = world->create();
			ent2->assign<Transform2D>(glm::vec2(400., 500.), 0.0f, 1.0f);
			ent2->assign<Sprite>("Textures/Buttons/Start/Start1.png", glm::vec3(1., 1., 1.),true, glm::vec2(300, 80));
			ent2->assign<ScoreComponent>(1);
			ent2->assign<UIComponent>("StartButton", "Menu");
			ent2->assign<ButtonComponent>("Start", "Textures/Buttons/Start/Start1.png", "Textures/Buttons/Start/Start2.png", "Textures/Buttons/Start/Start3.png", "Textures/Buttons/Start/Start4.png", "Textures/Buttons/Start/Start5.png");
		}
		else if (page == "Game") {
			//destroy all UI elements
			world->each<UIComponent>([&](Entity* ent, ComponentHandle<UIComponent> uiComp) {
				world->destroy(ent);
				});
			game->pause = false;
			game->score = 0;
			Entity* ent = world->create();
			ent->assign<Transform2D>(glm::vec2(400., 400.), 0.0f, 1.0f);
			ent->assign<Sprite>("Textures/text/score_text.png", glm::vec3(1., 1., 1.));
			ent->assign<UIComponent>("ScoreText", "Game");

			Entity* ent2 = world->create();
			ent2->assign<Transform2D>(glm::vec2(400., 400.), 0.0f, 1.0f);
			ent2->assign<Sprite>("Textures/text/0.png", glm::vec3(1., 1., 1.));
			ent2->assign<ScoreComponent>(1);
			ent2->assign<UIComponent>("ScoreVal", "Game");
		}
		else if (page == "pause") {
			game->pause = true;
			Entity* ent2 = world->create();
			ent2->assign<Transform2D>(glm::vec2(400., 500.), 0.0f, 1.0f);
			ent2->assign<Sprite>("Textures/Buttons/Restart/Restart1.png", glm::vec3(1., 1., 1.), true, glm::vec2(300, 80));
			ent2->assign<UIComponent>("ResumeButton", "Menu");
			ent2->assign<ButtonComponent>("Resume", "Textures/Buttons/Restart/Restart1.png", "Textures/Buttons/Restart/Restart2.png", "Textures/Buttons/Restart/Restart3.png", "Textures/Buttons/Restart/Restart4.png", "Textures/Buttons/Restart/Restart5.png");

		}
		else if (page == "GameOver") {
			game->pause = true;
			Entity* ent = world->create();
			ent->assign<Transform2D>(glm::vec2(400., 400.), 0.0f, 1.0f);
			ent->assign<Sprite>("Textures/text/game_over.png", glm::vec3(1., 1., 1.));
			ent->assign<UIComponent>("GameOver", "Menu");

			Entity* ent2 = world->create();
			ent2->assign<Transform2D>(glm::vec2(400., 500.), 0.0f, 1.0f);
			ent2->assign<Sprite>("Textures/Buttons/Restart/Restart1.png", glm::vec3(1., 1., 1.), true, glm::vec2(300, 80));
			ent2->assign<ScoreComponent>(1);
			ent2->assign<UIComponent>("RestartButton", "Menu");
			ent2->assign<ButtonComponent>("Restart", "Textures/Buttons/Restart/Restart1.png", "Textures/Buttons/Restart/Restart2.png", "Textures/Buttons/Restart/Restart3.png", "Textures/Buttons/Restart/Restart4.png", "Textures/Buttons/Restart/Restart5.png");

		}
		else if (page == "Tutorial") {

		}
		changePage = false;
	}

	

	if (page == "Menu") {
		game->pause = true;
		//if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
		//	page = "Game";
		//	changePage = true;
		//}
		startButtonCheck(deltaTime);
	}
	else if (page == "Game") {
		
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			if (!firstClick) {
				game->freeCam = !game->freeCam;
				firstClick = true;
			}

		}
		else {
			firstClick = false;
		}
		int digits = 0;
		int scoreDigits = 0;
		int tmp = game->score;
		while (tmp > 0) {
			tmp /= 10;
			scoreDigits++;
		}
		if (scoreDigits == 0) {
			scoreDigits = 1;
		}
		world->each<ScoreComponent>([&](Entity* ent, ComponentHandle<ScoreComponent> scoreComp) {
			digits = (digits > scoreComp->digit) ? digits : scoreComp->digit;
			});
		// spawn another digit if needed
		if (digits < scoreDigits) {
			Entity* ent = world->create();
			ent->assign<Transform2D>(glm::vec2(400., 400.), 0.0f, 1.0f);
			ent->assign<Sprite>("Textures/text/0.png", glm::vec3(1., 1., 1.));
			ent->assign<ScoreComponent>(scoreDigits);
			ent->assign<UIComponent>("ScoreVal", "Game");
		}


		world->each<ScoreComponent>([&](Entity* ent, ComponentHandle<ScoreComponent> scoreComp) {
			// position digits
			int digitNum = scoreComp->digit;

			// Extract individual digits from game->score
			int scoreCopy = game->score;
			for (int i = 1; i < digitNum; i++) {
				int digit = scoreCopy % 10;
				scoreCopy /= 10;
			}

			int digitVal = scoreCopy % 10;
			// Set the position of the digit
			ent->get<Transform2D>()->position.x = 400. + ((scoreDigits - digitNum) * 20);

			// Set the Sprite filepath based on digitVal
			switch (digitVal) {
			case 0:
				ent->get<Sprite>()->filepath = "Textures/text/0.png";
				break;
			case 1:
				ent->get<Sprite>()->filepath = "Textures/text/1.png";
				break;
			case 2:
				ent->get<Sprite>()->filepath = "Textures/text/2.png";
				break;
			case 3:
				ent->get<Sprite>()->filepath = "Textures/text/3.png";
				break;
			case 4:
				ent->get<Sprite>()->filepath = "Textures/text/4.png";
				break;
			case 5:
				ent->get<Sprite>()->filepath = "Textures/text/5.png";
				break;
			case 6:
				ent->get<Sprite>()->filepath = "Textures/text/6.png";
				break;
			case 7:
				ent->get<Sprite>()->filepath = "Textures/text/7.png";
				break;
			case 8:
				ent->get<Sprite>()->filepath = "Textures/text/8.png";
				break;
			case 9:
				ent->get<Sprite>()->filepath = "Textures/text/9.png";
				break;
			default:
				ent->get<Sprite>()->filepath = "Textures/text/0.png";
				break;
			}

			});

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			game->pause = true;
			this->changePage = true;
			this->page = "pause";
		}
	}
	else if (page == "pause") {
		startButtonCheck(deltaTime);
	}



}

void UIScript::startButtonCheck(float deltaTime) {

	// check button pressed
	world->each<ButtonComponent>([&](Entity* other_ent, ComponentHandle<ButtonComponent> button) {
		
		//if (button->id < 0) return;
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !button->clicked) {
			glm::vec2 p1 = other_ent->get<Transform2D>()->position;
			ComponentHandle<Sprite> sprite = other_ent->get<Sprite>();
			double mouseX, mouseY;
			glfwGetCursorPos(window, &mouseX, &mouseY);

			// Check if the click is within the entity bounds
			if (mouseX > p1.x - sprite->size.x / 2 && mouseX < p1.x + sprite->size.x / 2 &&
				mouseY >  p1.y - sprite->size.y / 2 && mouseY < p1.y + sprite->size.y / 2) {
				// The mouse click is within the entity bounds
				cout << "button clicked" << endl;
				button->clicked = true;
				//other_ent->get<SoundComponent>()->playSound = true;
			}

		}
		if (button->clicked) {
			ComponentHandle<Sprite> sprite = other_ent->get<Sprite>();

			if (button->ticksSinceClicked >= 120) {
				sprite->filepath = button->frame1;
				//button->ticksSinceClicked = 0;
				if (button->id == "Start") {
					page = "Game";
					changePage = true;
				}
				else if (button->id == "Resume") {
					page = "Game";
					world->each<GameController>([&](Entity* ent, ComponentHandle<GameController> gameController) {gameController->pause = false; });
					world->destroy(other_ent);
				}
			}
		}

		});

	// animate buttons
	world->each<ButtonComponent>([&](Entity* other_ent, ComponentHandle<ButtonComponent> button) {
		if (button->clicked) {
			//cout << "button click animation" << endl;
			button->ticksSinceClicked += deltaTime / 1.5f;
			ComponentHandle<Sprite> sprite = other_ent->get<Sprite>();

			int currentFrame = button->ticksSinceClicked;

			if ((currentFrame >= 0 && currentFrame < 10) || (currentFrame >= 80 && currentFrame < 90)) {
				sprite->filepath = button->frame1;
			}
			else if (currentFrame >= 10 && currentFrame < 20 || ((currentFrame >= 70 && currentFrame < 80))) {
				sprite->filepath = button->frame2;
			}
			else if ((currentFrame >= 20 && currentFrame < 30) || (currentFrame >= 60 && currentFrame < 70)) {
				sprite->filepath = button->frame3;
			}
			else if ((currentFrame >= 30 && currentFrame < 40) || (currentFrame >= 50 && currentFrame < 60)) {
				sprite->filepath = button->frame4;
			}
			else if (currentFrame >= 40 && currentFrame < 50) {
				sprite->filepath = button->frame5;
			}
			else if (currentFrame >= 200) {
				sprite->filepath = button->frame1;
				button->clicked = false;
				button->ticksSinceClicked = 0;
			}
		}
		});
}