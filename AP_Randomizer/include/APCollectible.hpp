#pragma once

using namespace RC::Unreal;

namespace Pseudoregalia_AP {
	class APCollectible {
	public:
		APCollectible(std::string new_map, RC::Unreal::FVector new_position, int64_t new_id) {
			map = new_map;
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
			return map;}

		int64_t GetID() {
			return id;}

		FVector GetPosition() {
			return position;}

	private:
		std::string map;
		int64_t id;
		RC::Unreal::FVector position;
		bool checked;
	};
}