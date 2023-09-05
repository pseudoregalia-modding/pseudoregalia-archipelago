#pragma once
#include "APClient.hpp"

namespace Pseudoregalia_AP {


	class GoatManager {

	public:
		GoatManager(APClient*);

	private:
		void GetCheck(int);
	};
}