#include "Script.h"

Script::Script() {

}

Script::Script(GLFWwindow* window, World* world, Entity* entity) {
	this->window = window;
	this->entity = entity;
	this->world = world;
}

void Script::startScript()
{
	
	cout << "start" << endl;
	
}

void Script::tickScript(float deltaTime)
{
	cout << "update" << endl;
}

void Script::Delete() {
	delete this;
}