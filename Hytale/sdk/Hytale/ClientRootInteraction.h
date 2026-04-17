#pragma once
#include "sdk/BaseDataTypes/HytaleString.h"
#include "sdk/BaseDataTypes/Object.h"

struct ClientRootInteraction : Object {
	HytaleString* ID; 								//0x08
	void* RootInteraction; 							//0x10 Hytale.Protocol.RootInteraction
	void* Operations; 								//0x18 HytaleClient.Data.ClientInteraction.ClientRootInteraction+Operation[]
	void* Rules; 									//0x20 HytaleClient.Data.ClientInteraction.ClientInteraction+ClientInteractionRules
	void* unknown_0x28; 							//0x28 System.Int32
	int interactionID;
};