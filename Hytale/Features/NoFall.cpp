#include "NoFall.h"

#include <iostream>

#include "../Util/Util.h"
#include "Hooks/Hooks.h"

void NoFall::PlayerMove(MoveCycleEvent& event) {
	if (event.dmc.Velocity.y < -20.0f)
		event.dmc.Velocity.y = -20.0f;

	
}
bool NoFall::CanExecute() {
	return Util::getLocalPlayer() != nullptr;
}