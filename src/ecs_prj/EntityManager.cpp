#include "EntityManager.h"
#include "Entity.h"

EntityManager::EntityManager()
{

}

Entity* EntityManager::addEntity()
{
	auto c = new Entity();

	entities.push_back(c);

	return c;
}

void EntityManager::update()
{	
	//The basic cycle of every entity
	for (int i = 0; i < entities.size(); i++) {
		auto e = entities[i];
		//if it has to be destoyed, we destroy it
		if (e->destroyed()) {
			delete e;
			entities.erase(entities.begin() + i);
		}//If it is active, we update it
		else if (e->isActive()) e->update(); 
	}
}