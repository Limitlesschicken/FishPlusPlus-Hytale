#pragma once

#include "ICameraController.h"

struct CameraModule {
	char pad_0000[32]; //0x0000
	ICameraController* ICameraController; //0x0020
	char pad_0028[1024]; //0x0028

};