#pragma once
#include "Math/Vector3.h"

struct RaycastHit {
	Vector3 BlockPosition;				// 0x00
	Vector3 BlockPositionNoFiller;		// 0x0C
	Vector3 BlockOrigin;				// 0x18
	Vector3 StartPosition;				// 0x24
	Vector3 HitPosition;				// 0x30
	Vector3 Normal;						// 0x3C
	Vector3 BlockNormal;				// 0x48
	Vector3 TextureCoord;				// 0x54
	int BlockID;						// 0x60
	int Rotation;						// 0x64
	char pad_0x68[0x18];				// 0x68
	float Distance;						// 0x80
};