#pragma once
#include <vector>

namespace Pseudoregalia_AP {
	class Engine {
	public:
		static void ExecuteInGameThread(void (*)());
		static void OnTick(UObject*);
	private:
		static std::vector<void (*)()> function_queue;
	};
}