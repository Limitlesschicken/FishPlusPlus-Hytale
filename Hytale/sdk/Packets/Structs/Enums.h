#pragma once
#include "Math/Vector3.h"

typedef unsigned char byte;

enum Rotation : byte {
	None,
	Ninety,
	OneEighty,
	TwoSeventy
};
enum InteractionType : byte { // Struct from pEric
	kInteractionTypePrimary = 0,
	kInteractionTypeSecondary = 1,
	kInteractionTypeAbility1 = 2,
	kInteractionTypeAbility2 = 3,
	kInteractionTypeAbility3 = 4,
	kInteractionTypeUse = 5,
	kInteractionTypePick = 6,
	kInteractionTypePickup = 7,
	kInteractionTypeCollisionEnter = 8,
	kInteractionTypeCollisionLeave = 9,
	kInteractionTypeCollision = 10,
	kInteractionTypeEntityStatEffect = 11,
	kInteractionTypeSwapTo = 12,
	kInteractionTypeSwapFrom = 13,
	kInteractionTypeDeath = 14,
	kInteractionTypeWielding = 15,
	kInteractionTypeProjectileSpawn = 16,
	kInteractionTypeProjectileHit = 17,
	kInteractionTypeProjectileMiss = 18,
	kInteractionTypeProjectileBounce = 19,
	kInteractionTypeHeld = 20,
	kInteractionTypeHeldOffhand = 21,
	kInteractionTypeEquipped = 22,
	kInteractionTypeDodge = 23,
	kInteractionTypeGameModeSwap = 24
};

enum InteractionState : byte { // Struct from pEric
	kInteractionStateFinished = 0,
	kInteractionStateSkip = 1,
	kInteractionStateItemChanged = 2,
	kInteractionStateFailed = 3,
	kInteractionStateNotFinished = 4
};
enum MovementDirection : byte { // Struct from pEric
	kMovementDirectionNone = 0,
	kMovementDirectionForward = 1,
	kMovementDirectionBack = 2,
	kMovementDirectionLeft = 3,
	kMovementDirectionRight = 4,
	kMovementDirectionForwardLeft = 5,
	kMovementDirectionForwardRight = 6,
	kMovementDirectionBackLeft = 7,
	kMovementDirectionBackRight = 8
};
enum ApplyForceState : byte { // Struct from pEric
	kApplyForceStateWaiting = 0,
	kApplyForceStateGround = 1,
	kApplyForceStateCollision = 2,
	kApplyForceStateTimer = 3
};
enum BlockFace : byte { // Struct from pEric
	kBlockFaceNone = 0,
	kBlockFaceUp = 1,
	kBlockFaceDown = 2,
	kBlockFaceNorth = 3,
	kBlockFaceSouth = 4,
	kBlockFaceEast = 5,
	kBlockFaceWest = 6
};