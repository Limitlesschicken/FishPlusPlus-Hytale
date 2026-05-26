/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include <type_traits>

struct ClientMovementStates {
	bool IsIdle; //0x0000
	bool IsHorizontalIdle; //0x0001
	bool IsJumping; //0x0002
	bool IsFlying; //0x0003
	bool IsSprinting; //0x0004
	bool IsWalking; //0x0005
	bool IsCrouching; //0x0006
	bool IsForcedCrouching; //0x0007
	bool IsFalling; //0x0008
	bool unknown; //0x0009
	bool IsClimbing; //0x000A
	bool IsInFluid; //0x000B
	bool IsSwimming; //0x000C
	bool IsSwimJumping; //0x000D
	bool IsOnGround; //0x000E
	bool IsEntityCollided; //0x000F
	bool IsMantling; //0x0010
	bool IsSliding; //0x0011
	bool IsMounting; //0x0012
	bool IsRolling; //0x0013
	//I think there are some more after this but i dont think its important to me


	template <typename T>
	bool Equals(T obj) {
		return (std::is_same<T, ClientMovementStates>::value);
	}
	
	bool Equals(ClientMovementStates other) {
		return IsIdle == other.IsIdle && IsHorizontalIdle == other.IsHorizontalIdle && IsJumping == other.IsJumping && IsFlying == other.IsFlying && IsSprinting == other.IsSprinting && IsWalking == other.IsWalking && IsCrouching == other.IsCrouching && IsForcedCrouching == other.IsForcedCrouching && IsFalling == other.IsFalling && unknown == other.unknown && IsClimbing == other.IsClimbing && IsInFluid == other.IsInFluid && IsSwimming == other.IsSwimming && IsSwimJumping == other.IsSwimJumping && IsOnGround == other.IsOnGround && IsEntityCollided == other.IsEntityCollided && IsMantling == other.IsMantling && IsSliding == other.IsSliding && IsMounting == other.IsMounting && IsRolling == other.IsRolling;
	}
};