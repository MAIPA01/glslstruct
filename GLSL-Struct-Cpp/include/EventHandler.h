#pragma once
#include <queue>
#include <functional>

namespace glsl::extra {
	template<typename Ret, typename... Args> using Func = std::function<Ret(Args...)>;
	template<typename... Args> using Action = Func<void, Args...>;
	using Method = Action<>;

	template<typename Ret, typename... Args> using CFunc = Ret(*)(Args...);
	template<typename... Args> using CAction = CFunc<void, Args...>;
	using CMethod = CAction<>;

	template<typename... Args>
	struct Event {
		size_t ID;
		Action<Args...> Action;
	};

	template<typename... Args>
	class EventHandler {
		using eventType = Event<Args...>;
		using eventsType = std::vector<eventType>;
		using idType = size_t;
		using removedIdsType = std::queue<idType>;
		using eventAction = Action<Args...>;

	private:
		eventsType _events = eventsType();
		idType _freeId = idType();
		removedIdsType _removedIds = removedIdsType();

	public:
		EventHandler() = default;
		virtual ~EventHandler() = default;

		constexpr idType AddCallback(const eventAction& callback) {
			Event e{
				.ID = 0,
				.Action = callback
			};
			if (_removedIds.size() > 0) {
				e.ID = _removedIds.front();
				_removedIds.pop();
			}
			else {
				e.ID = _freeId++;
			}
			_events.push_back(e);
			return e.ID;
		}

		bool RemoveCallback(idType callbackId) {
			for (size_t i = 0; i < _events.size(); ++i) {
				if (_events[i].ID == callbackId) {
					_events.erase(_events.begin() + i);
					_removedIds.push(callbackId);
					return true;
				}
			}

			return false;
		}
		constexpr void RemoveAllCallbacks() {
			_events.clear();
			_freeId = 0;
			while (!_removedIds.empty()) _removedIds.pop();
		}

		void Invoke(Args... args) const {
			for (eventType event : _events) {
				event.Action(args...);
			}
		}

		constexpr idType operator+=(const eventAction& callback) {
			return AddCallback(callback);
		}

		constexpr bool operator-=(idType callbackId) {
			return RemoveCallback(callbackId);
		}

		constexpr void operator()(Args... args) const {
			Invoke(args...);
		}
	};
	using MethodEventHandler = EventHandler<>;
}