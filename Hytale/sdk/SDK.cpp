#include "SDK.h"

std::vector<EntityData> getEntities(Entity* localPlayer) {
	GameInstance* gameInstance = Util::getGameInstance();
	ValidPtrEmpty(gameInstance);

	EntityStoreModule* entityStoreModule = gameInstance->EntityStoreModule;
	ValidPtrEmpty(entityStoreModule);

	int entityCount = entityStoreModule->entityCount;
	Array<Entity*>* entityArray = entityStoreModule->entityArray;

	std::vector<EntityData> entities;
	for (int i = 0; i < entityCount; i++) {
		Entity* entity = entityArray->get(i);
		ValidPtrLoop(entity);

		Entity::EntityPlayerType entityType = entity->entityPlayerType;

		EntityAssetStruct* assetStruct = entity->AssetNameStruct;
		ValidPtrLoop(assetStruct);

		HytaleString* entityString = assetStruct->entityString;
		ValidPtrLoop(assetStruct);

		EntityData data;
		data.entityPtr = entity;

		if (entityType == Entity::EntityPlayerType::Player)
			data.name = entity->Name->getName();
		else
			data.name = entityString->getName();

		data.entityType = entity->entityType;
		data.position = entity->Position;
		data.isLocalPlayer = (entity == localPlayer);

		//printf("Entity %d: %s - 0x%llX\n", i, name.c_str(), entity);

		entities.push_back(data);
	}
	return entities;
}


void SDK::Main() {
	global_mutex.lock();
	entities = getEntities(Util::getLocalPlayer());
	global_mutex.unlock();
}