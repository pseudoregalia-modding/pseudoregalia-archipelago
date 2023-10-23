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

		bool IsChecked() const {
			return checked;}

		FVector GetPosition() const {
			return position;}

		std::string RequiredOption() const {
			return required_option;
		}

	private:
		FVector position;
		bool checked;
		// TODO: Change this to a map or something so both true and false options can be checked
		std::string required_option;
	};
}