#include "Speed.h"

#include <Windows.h>
#include <cmath>
#include <iostream>

#include "../Util/Util.h"

void Speed::PlayerMove(MoveCycleEvent& event) {

	//std::cout << dmc << "\n";
	/*
	float yawRad = Util::getLocalPlayer()->yawRad;
	float forwardX = -sin(yawRad);
	float forwardZ = -cos(yawRad);

	float strafeX = forwardZ;
	float strafeZ = -forwardX;

	if (GetAsyncKeyState('W')) {
		*wishDirection += Vector3(forwardX * 2.0f, wishDirection->y, forwardZ * 5.0f);
	}

	if (GetAsyncKeyState('S')) {
		*wishDirection += Vector3(-forwardX * 2.0f, wishDirection->y, -forwardZ * 5.0f);
	}

	if (GetAsyncKeyState('A')) {
		*wishDirection += Vector3(strafeX * 2.0f, wishDirection->y, strafeZ * 5.0f);
	}

	if (GetAsyncKeyState('D')) {
		*wishDirection += Vector3(-strafeX * 2.0f, wishDirection->y, -strafeZ * 5.0f);
	}
	*/
}

bool Speed::CanExecute() {
	return Util::getLocalPlayer() != nullptr;
}