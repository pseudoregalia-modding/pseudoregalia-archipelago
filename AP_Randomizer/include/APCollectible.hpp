#pragma once

using namespace RC::Unreal;

namespace GameData {
	class APCollectible {
	public:
		APCollectible(std::string new_name, RC::Unreal::FVector new_position, int64_t new_id) {
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
		RC::Unreal::FVector position;
		bool checked;
	};
}