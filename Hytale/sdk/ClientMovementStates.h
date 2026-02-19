/*
 * This file is part of the FishPlusPlus distribution (https://github.com/Limitlesschicken/FishPlusPlus).
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include <type_traits>

struct ClientMovementStates {
	bool IsIdle;
	bool IsHorizontalIdle;
	bool IsJumping;
	bool IsFlying;
	bool IsSprinting;
	bool IsWalking;
	bool IsCrouching;
	bool IsForcedCrouching;
	bool IsFalling;
	bool IsClimbing;
	bool IsInFluid;
	bool IsSwimming;
	bool IsSwimJumping;
	bool IsOnGround;
	bool IsEntityCollided;
	bool IsMantling;
	bool IsSliding;
	bool IsMounting;
	bool IsRolling;

	template <typename T>
	bool Equals(T obj) {
		return (std::is_same<T, ClientMovementStates>::value);
	}
	
	bool Equals(ClientMovementStates other) {
		return IsIdle == other.IsIdle && IsHorizontalIdle == other.IsHorizontalIdle && IsJumping == other.IsJumping && IsFlying == other.IsFlying && IsSprinting == other.IsSprinting && IsWalking == other.IsWalking && IsCrouching == other.IsCrouching && IsForcedCrouching == other.IsForcedCrouching && IsFalling == other.IsFalling && IsClimbing == other.IsClimbing && IsInFluid == other.IsInFluid && IsSwimming == other.IsSwimming && IsSwimJumping == other.IsSwimJumping && IsOnGround == other.IsOnGround && IsEntityCollided == other.IsEntityCollided && IsMantling == other.IsMantling && IsSliding == other.IsSliding && IsMounting == other.IsMounting && IsRolling == other.IsRolling;
	}
};