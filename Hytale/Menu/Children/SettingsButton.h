#pragma once

#include "../Component.h"

#include "FeatureDispatcher/Setting.h"

template<typename T>
class SettingButton : public Component {
public:
	Setting<T> setting;
};