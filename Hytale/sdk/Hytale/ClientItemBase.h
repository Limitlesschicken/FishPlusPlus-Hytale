#pragma once

struct ClientItemBase {
	char pad_0000[8];                                   //0x00
	HytaleString* Id;									//0x08 System.String
	void* Categories;									//0x10 System.String[]
	char pad_0018[8];                                   //0x18 
	HytaleString* Set;									//0x20 System.String
	void* ResourceTypes;								//0x28 HytaleClient.Data.Items.ClientItemResourceType[]
	char pad_0030[32];                                  //0x30 
	HytaleString* ItemIconPNG;                          //0x50 System.String
	char pad_0058[8];                                   //0x58 
	HytaleString* StringPtr3;                           //0x60 System.String
	void* ClientItemPlayerAnimations;                   //0x68 HytaleClient.Data.Items.ClientItemPlayerAnimations
	void* ItemUtility;                                  //0x70 Hytale.Protocol.ItemUtility
	char pad_0078[56];                                  //0x78 
	void* ClientItemTranslationProperties;              //0xB0 HytaleClient.Data.Items.ClientItemTranslationProperties
	HytaleString* ActualItemIconPNG;                    //0xB8 System.String
	void* ItemEntityConfig;                             //0xC0 Hytale.Protocol.ItemEntityConfig
	void* Interactions;									//0xC8 System.Collections.Generic.Dictionary`2[Hytale.Protocol.InteractionType,System.Int32]
	void* InteractionVars;                              //0xD0 System.Collections.Generic.Dictionary`2[System.String,System.Int32]
	void* InteractionConfiguration;                     //0xD8 Hytale.Protocol.InteractionConfiguration
	char pad_00E0[16];                                  //0xE0 
	void* Int32Array;                                   //0xF0 System.Int32[]
	char pad_00F8[0x24];                                //0xF8
	int intUnknown;										//0x11C
	int intUnknown2;									//0x120
	int intUnknown3;									//0x124
	float Durability;									//0x128
	int BlockId;										//0x12C
	int maxStackSize;									//0x130

	bool IsBlock() {
		return BlockId != 0;
	}

	void DBGPrint() {
		printf("ClientItemBase at: 0x%p", this);
		if (Id)
			printf("Id: %s", Id->getString().c_str());
		if (Set)
			printf("Set: %s", Set->getString().c_str());
		if (ItemIconPNG)
			printf("ItemIconPNG: %s", ItemIconPNG->getString().c_str());
		if (StringPtr3)
			printf("StringPtr3: %s", StringPtr3->getString().c_str());
		if (ActualItemIconPNG)
			printf("ActualItemIconPNG: %s", ActualItemIconPNG->getString().c_str());

		printf("Durability: %f", Durability);
		printf("BlockId: %d", BlockId);
		printf("maxStackSize: %d", maxStackSize);
		printf("intUnknown: %d", intUnknown);
		printf("intUnknown2: %d", intUnknown2);
		printf("intUnknown3: %d", intUnknown3);
	}

}; // Size: 0x134