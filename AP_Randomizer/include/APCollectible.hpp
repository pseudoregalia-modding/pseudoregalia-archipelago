#ifndef APCollectible_H
#define APCollectible_H
#include <Unreal/World.hpp>

using namespace RC::Unreal;

namespace Pseudoregalia_AP {
	class APCollectible {
	public:
		std::string map;
		int id;
		RC::Unreal::FVector position;
		bool checked;

		APCollectible(std::string new_map, RC::Unreal::FVector new_position, long new_id) {
			map = new_map;
			position = new_position;
			id = new_id;
		}
	};
}

#endif