#pragma once
#include <vector>

namespace Pseudoregalia_AP {
	class Engine {
	public:
		static void ExecuteInGameThread(void (*)());
		static void OnTick();
	private:
		static std::vector<void (*)()> function_queue;
	};
}