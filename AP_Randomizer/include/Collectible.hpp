#pragma once
#include "Unreal/UnrealCoreStructs.hpp"

namespace GameData {
	using RC::Unreal::FVector;

	class Collectible {
	public:
		Collectible(std::string new_name, FVector new_position, int64_t new_id) {
			name = new_name;
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

		std::string GetMap() {
			return name;}

		int64_t GetID() {
			return id;}

		FVector GetPosition() {
			return position;}

	private:
		std::string name;
		int64_t id;
		FVector position;
		bool checked;
	};
}