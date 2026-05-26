#include "TestFeature.h"

#include "sdk/Packets/SyncInteractionChains.h"

void TestFeature::OnFrame() {
}

void TestFeature::OnActivate() {
	
}
void TestFeature::Initialize() {
	Util::log("Initialized Test feature");
	RegisterEvent(this);
}