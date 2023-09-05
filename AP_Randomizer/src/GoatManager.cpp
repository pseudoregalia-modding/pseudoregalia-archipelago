#pragma once
#include "GoatManager.hpp"

namespace Pseudoregalia_AP {
	APClient* client;

	GoatManager::GoatManager(APClient* new_client) {
		client = new_client;
	}

	

	void GetCheck(int id) {
		// hook into collectible collision function and have it callback to this.
		client->SendCheck(id);
	}
}