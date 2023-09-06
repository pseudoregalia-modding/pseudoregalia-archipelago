#include <Unreal/World.hpp>

using namespace RC::Unreal;

namespace Pseudoregalia_AP {
	class APCollectible {
	public:
		APCollectible(std::string new_map, FVector position, long new_id);

		std::string map;
		int id;
		FVector position;
		bool checked;
	};
}