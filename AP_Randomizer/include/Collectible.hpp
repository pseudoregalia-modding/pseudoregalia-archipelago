#pragma once
#include "Unreal/UnrealCoreStructs.hpp"

namespace GameData {
	using RC::Unreal::FVector;

	class Collectible {
	public:
		Collectible(FVector new_position) {
			position = new_position;
		}

		Collectible(FVector new_position, std::tuple<FVector, std::string, int> new_alternate_position) {
			position = new_position;
			alternate_position = new_alternate_position;
		}

		FVector GetPosition(std::unordered_map<std::string, int> option_set) const {
			if (alternate_position) {
				const auto& [pos, option_name, option_value] = *alternate_position;
				if (option_set.at(option_name) == option_value) {
					return pos;
				}
			}
			return position;
		}

	private:
		FVector position;
		std::optional<std::tuple<FVector, std::string, int>> alternate_position;
	};
}