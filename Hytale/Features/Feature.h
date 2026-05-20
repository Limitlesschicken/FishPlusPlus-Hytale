/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include <string>
#include <vector>
#include <concepts>

#include "Setting.h"

#include "Util/InputSystem.h"

#include "Events/EventRegister.h"

template<typename T>
concept HasDoMoveCycle = requires(T t, DefaultMovementController* dmc, Vector3& offset) {
	t->OnMoveCycle(dmc, offset);
};

template<typename T>
concept HasRender3D = requires(T t, Renderer3D& renderer) {
	t->OnRender3D(renderer);
};

template<typename T>
concept HasRender2D = requires(T t) {
	t->OnRender2D();
};

template<typename T>
concept HasPacketRecieve = requires(T t, Object* packet, PacketIndex& index, bool& cancel) {
	t->OnPacketRecieved(packet, index, cancel);
};

template<typename T>
concept HasFrame = requires(T t) {
	t->OnFrame();
};

template<typename T>
concept HasPacketSend = requires(T t, Object * packet, PacketIndex & index, bool& cancel) {
	t->OnPacketSend(packet, index, cancel);
};

class Feature {
public:
	Feature(std::string name);

	virtual bool CanExecute();
	virtual void OnActivate();
	virtual void OnDeactivate();
	virtual void Initialize();
	
	void CreateForcedKeybind();
	
	template<typename T, typename... Args>
	T* RegisterSetting(Args&&... args) {
		auto ptr = std::make_unique<T>(std::forward<Args>(args)...);
		T* raw = ptr.get();
		m_settings.push_back(std::move(ptr));
		return raw;
	}

	template<typename T>
	void RegisterEvent(T feature) {
		if constexpr (HasDoMoveCycle<T>) {
			EventRegister::DoMoveCycleEvent.Subscribe([feature](DefaultMovementController* dmc, Vector3& dir) {
				if (feature->IsActive() && feature->CanExecute())
					feature->OnMoveCycle(dmc, dir);
				});
		}

		if constexpr (HasRender2D<T>) {
			EventRegister::Render2DEvent.Subscribe([feature]() {
				if (feature->IsActive() && feature->CanExecute())
					feature->OnRender2D();
				});
		}

		if constexpr (HasRender3D<T>) {
			EventRegister::Render3DEvent.Subscribe([feature](Renderer3D& renderer) {
				if (feature->IsActive() && feature->CanExecute())
					feature->OnRender3D(renderer);
				});
		}

		if constexpr (HasPacketRecieve<T>) {
			EventRegister::PacketRecieveEvent.Subscribe([feature](Object* packet, PacketIndex& index, bool& cancel) {
				if (feature->IsActive() && feature->CanExecute())
					feature->OnPacketRecieved(packet, index, cancel);
				});
		}

		if constexpr (HasFrame<T>) {
			EventRegister::FrameEvent.Subscribe([feature]() {
				if (feature->IsActive() && feature->CanExecute())
					feature->OnFrame();
				});
		}

		if constexpr (HasPacketSend<T>) {
			EventRegister::PacketSendEvent.Subscribe([feature](Object* packet, PacketIndex& index, bool& cancel) {
				if (feature->IsActive() && feature->CanExecute())
					feature->OnPacketSend(packet, index, cancel);
				});
		}
	}

	ISetting* GetSettingFromName(std::string name) {
		for (auto& setting : this->m_settings) {
			if (setting->GetName() == name)
				return setting.get();
		}
		return nullptr;
	}

	[[nodiscard]] const std::string& GetName() const { return m_name; }
	[[nodiscard]] const std::string& GetCategory() const { return m_category; }
	[[nodiscard]] const std::vector<std::unique_ptr<ISetting>>& GetSettings() const { return m_settings; }
	[[nodiscard]] SDL_Scancode GetKeybind() const { return keybind; }
	[[nodiscard]] bool IsActive() const { return active; }

	void SetKeybind(SDL_Scancode key) {this->keybind = key;}

	void setActive(bool active) {
		this->active = active;
		if (active) {
			OnActivate();
		}
		else {
			OnDeactivate();
		}
	}

	void ToggleState() {
		setActive(!active);
	}


	void setCategory(std::string category);
	
protected:
	bool active;
	SDL_Scancode keybind = SDL_SCANCODE_UNKNOWN;
	std::string m_name;
	std::string m_category;

	std::vector<std::unique_ptr<ISetting>> m_settings;
};