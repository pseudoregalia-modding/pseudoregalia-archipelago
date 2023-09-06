#include "MapManager.hpp"
#include "Unreal/UObjectGlobals.hpp"
#include "Unreal/UClass.hpp"

using namespace RC;
using namespace RC::Unreal;

namespace Pseudoregalia_AP {

	void SpawnActors(std::vector<APCollectible> collectible_vector) {

        const wchar_t* collectible_class_name = STR("/Game/Mods/AP_Randomizer/BP_APCollectible.BP_APCollectible_C");
        static UClass* collectible_class = UObjectGlobals::StaticFindObject<UClass*>(nullptr, nullptr, collectible_class_name);

        if (!collectible_class) {
            // return an error
        }

        auto World = static_cast<UWorld*>(UObjectGlobals::FindFirstOf(STR("World")));        
        for (int i = 0; i < collectible_vector.size(); i++) {
            World->SpawnActor(
                collectible_class,
                &collectible_vector[i].position
            );
        }
	}
}