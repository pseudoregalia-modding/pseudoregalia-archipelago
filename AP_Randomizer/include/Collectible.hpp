#pragma once
#include "Unreal/UnrealCoreStructs.hpp"

namespace GameData {
	using RC::Unreal::FVector;

	class Collectible {
	public:
		Collectible(FVector new_position, int64_t new_id) {
			position = new_position;
			id = new_id;
			checked = false;
		}

		bool Check() {
			this->checked = true;
			return checked;
		}

		bool IsChecked() {
			return this->checked;}

		int64_t GetID() {
			return id;}

		FVector GetPosition() {
			return position;}

	private:
		int64_t id;
		FVector position;
		bool checked;
	};
}