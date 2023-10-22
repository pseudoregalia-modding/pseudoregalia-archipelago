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

		Collectible(FVector new_position, std::string new_option) {
			position = new_position;
			required_option = new_option;
			checked = false;
		}

		bool Check() {
			checked = true;
			return checked;
		}

		bool IsChecked() {
			return checked;}

		FVector GetPosition() {
			return position;}

		std::string RequiredOption(std::string input) {
			return required_option;
		}

	private:
		FVector position;
		bool checked;
		std::string required_option;
	};
}