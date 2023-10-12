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

		bool Check() {
			this->checked = true;
			return checked;
		}

		bool IsChecked() {
			return this->checked;}

		FVector GetPosition() {
			return position;}

	private:
		FVector position;
		bool checked;
	};
}