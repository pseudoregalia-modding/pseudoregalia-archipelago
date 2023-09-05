#pragma once
#include "GoatManager.hpp"

namespace Pseudoregalia_AP {
	APClient* client;

	GoatManager::GoatManager() {
	}

	

	void GetCheck(int id) {
		// hook into collectible collision function and have it callback to this.
		client->SendCheck(id);
	}
}