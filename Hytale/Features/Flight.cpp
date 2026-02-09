#include "Flight.h"

#include "../Util/Util.h"

void Flight::PlayerMove(MoveCycleEvent& event) {
	event.dmc.clientMovementStates.IsFlying = true;
}

void Flight::OnDeactivate() {
	Util::dmc->clientMovementStates.IsFlying = false;
}

bool Flight::CanExecute() {
	return Util::dmc != nullptr;
}