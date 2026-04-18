#pragma once
/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once
#include "Features/Feature.h"

class NoDeath : public Feature { // Inspiration from pEric & Ariorh1337
public:
	NoDeath() : Feature("NoDeath") {}

	bool CanExecute() override;
	void Initialize() override;
	void OnPacketRecieved(Object* packet, PacketIndex& index, bool& cancel);
};