#include "Animator.h"
#include <ecs_prj/Entity.h>

#include <OgreCamera.h>
#include "Transform.h"
#include "MeshRenderer.h"
#include <OgreNode.h>
#include <OgreAnimationState.h>
#include <OgreEntity.h>
#include <utils_prj/Timer.h>


namespace K_Engine {
	//Required
	std::string Animator::name = "Animator";

	Animator::Animator(Entity* e) : Component("Animator", e) {
		name = id; 
	}

	Animator::~Animator()
	{
	}

	std::string Animator::GetId()
	{
		return name;
	}

	void Animator::start()
	{
		// Obtenemos los componentes y entidades necesarios
		tr_ = entity->getComponent<Transform>();
		mesh_ = entity->getComponent<MeshRenderer>();
		node_ = mesh_->getOgreNode();
		ogreEntity_ = mesh_->getOgreEntity();

		// Recogemos todos los estados que traiga la malla
		animStatesMap_ = ogreEntity_->getAllAnimationStates();
	}

	void Animator::update(int frameTime)
	{
		// Actualizamos la animacion actual
		currentState_->animation->addTime(frameTime);

		// Miramos si hay que cambiar de estado de animacion
		manageAnimTransitions();
	}

	void Animator::setAnimBool(std::string anim, std::string condName, bool value)
	{
		animTransitionsMap_.at(anim).at(condName)->cond = value;
	}

	bool Animator::getAnimBool(std::string anim, std::string condName)
	{
		return animTransitionsMap_.at(anim).at(condName)->cond;
	}
	void Animator::manageAnimTransitions()
	{
		// ANYSTATE
		for (auto t : animTransitionsMap_.at("anyState"))
		{
			// Si alguna de las transiciones desde el anyState estan a true cambiamos el estado
			if (t.second->cond)
			{
				currentState_->animation = animStatesMap_->getAnimationState(t.second->nextState);
				return; // cortamos el metodo
			}
		}

		// CURRENTSTATE
		for (auto t : animTransitionsMap_.at(currentState_->name))
		{
			// Si alguna de las transiciones desde el currentState estan a true cambiamos el estado
			if (t.second->cond)
			{
				currentState_->animation = animStatesMap_->getAnimationState(t.second->nextState);
				return; // cortamos el metodo
			}
		}
	}
}