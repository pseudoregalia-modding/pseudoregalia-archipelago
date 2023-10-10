#pragma once
#include "Engine.hpp"

namespace Pseudoregalia_AP {
	void Engine::ExecuteInGameThread(void (*function)() ) {
		function_queue.push_back(function);
	}

	void Engine::OnTick() {
		for (auto& function : function_queue) {
			function();
		}
	}
}