#pragma once
#include "APCollectible.hpp"
#include "Unreal/UObjectGlobals.hpp"
#include "Unreal/UClass.hpp"

using namespace RC;
using namespace RC::Unreal;

namespace Pseudoregalia_AP {
	class MapManager {
	public:
		MapManager();
		void SpawnCollectibles(std::vector<APCollectible>);

	private:

	};

}