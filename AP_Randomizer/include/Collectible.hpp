#pragma once
#include "Unreal/UnrealCoreStructs.hpp"

namespace GameData {
	using RC::Unreal::FVector;

	class Collectible {
	public:
		Collectible(FVector new_position) {
			position = new_position;
			checked = false;
		}

		Collectible(FVector new_position, std::vector<std::pair<std::string, int>> new_options) {
			position = new_position;
			required_options = new_options;
			checked = false;
		}

		bool Check() {
			checked = true;
			return checked;
		}

		bool IsChecked() const {
			return checked;}

		FVector GetPosition() const {
			return position;}

		bool CanCreate(std::unordered_map<std::string, int> option_set) const {
			for (const auto& [option_name, option_value] : required_options) {
				if (option_set.at(option_name) != option_value) {
					return false;
				}
			}
			// Note that this always returns true if the collectible has no required options.
			return true;
		}

	private:
		FVector position;
		bool checked;
		std::vector<std::pair<std::string, int>> required_options;
	};
}