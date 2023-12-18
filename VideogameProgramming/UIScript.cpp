#include "UIScript.h"
#include <chrono>

void UIScript::startScript() {

}

void UIScript::tickScript(float deltaTime) {


	ComponentHandle<GameController> game;
	world->each<GameController>([&](Entity* ent, ComponentHandle<GameController> gameController) {game = gameController; });
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
	world->each<ScoreComponent>([&](Entity* ent, ComponentHandle<ScoreComponent> scoreComp) {
		digits = (digits > scoreComp->digit) ? digits : scoreComp->digit;
		});
	// spawn another digit if needed
	if (digits < scoreDigits) {
		Entity* ent = world->create();
		ent->assign<Transform2D>(glm::vec2(400., 400.), 0.0f, 1.0f);
		ent->assign<Sprite>("Textures/text/0.png", glm::vec3(1., 1., 1.));
		ent->assign<ScoreComponent>(scoreDigits);
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
        ent->get<Transform2D>()->position.x = 400. + ((scoreDigits-digitNum) * 20);

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


}