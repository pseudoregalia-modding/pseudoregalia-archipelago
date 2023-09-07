#pragma once
#include <Unreal/World.hpp>

using namespace RC::Unreal;

namespace Pseudoregalia_AP {
	class APCollectible {
	public:
		std::string map;
		int64_t id;
		RC::Unreal::FVector position;
		bool checked;

		APCollectible(std::string new_map, RC::Unreal::FVector new_position, int64_t new_id) {
			map = new_map;
			position = new_position;
			id = new_id;
			checked = true;
		}
	};
}