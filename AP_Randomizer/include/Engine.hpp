#pragma once
#include <vector>

namespace Pseudoregalia_AP {
	class Engine {
	public:
		static void ExecuteInGameThread(void (*)(UObject*));
		static void OnTick(UObject*);
	private:
		static std::vector<void (*)(UObject*)> function_queue;
	};
}