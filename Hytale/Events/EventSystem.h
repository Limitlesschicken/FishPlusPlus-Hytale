/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include <functional>
#include <vector>
#include <mutex>
#include <algorithm>

template<typename... Args>
class Event {
public:
	using CallbackType = std::function<void(Args...)>;
	using CallbackID = size_t;

private:
	struct CallbackData {
		CallbackID id;
		CallbackType callback;
	};

	std::vector<CallbackData> m_callbacks;
	std::mutex m_mutex;
	CallbackID m_nextID = 0;

public:
	// Register a callback and return its ID for later removal
	CallbackID Subscribe(CallbackType callback) {
		std::lock_guard<std::mutex> lock(m_mutex);
		CallbackID id = m_nextID++;
		m_callbacks.push_back({ id, std::move(callback) });
		return id;
	}

	// Remove a specific callback by ID
	bool Unsubscribe(CallbackID id) {
		std::lock_guard<std::mutex> lock(m_mutex);
		auto it = std::remove_if(m_callbacks.begin(), m_callbacks.end(),
			[id](const CallbackData& data) { return data.id == id; });
		
		if (it != m_callbacks.end()) {
			m_callbacks.erase(it, m_callbacks.end());
			return true;
		}
		return false;
	}

	// Call all registered callbacks
	void Invoke(Args... args) {
		m_mutex.lock();
		std::vector<CallbackData> callbacksCopy = m_callbacks;
		m_mutex.unlock();
		for (auto& callbackData : callbacksCopy) {
			callbackData.callback(args...);
		}
	}

	// Get number of subscribers
	size_t GetSubscriberCount() const {
		m_mutex.lock();
		size_t size = m_callbacks.size();
		m_mutex.unlock();
		return size;
	}

	// Clear all callbacks
	void Clear() {
		m_mutex.lock();
		m_callbacks.clear();
		m_mutex.unlock();
	}
};

// Example usage:
// Event<int, std::string> myEvent;
// auto id = myEvent.Subscribe([](int x, std::string s) { printf("%d: %s", x, s.c_str()); });
// myEvent.Invoke(42, "Hello");
// myEvent.Unsubscribe(id);
