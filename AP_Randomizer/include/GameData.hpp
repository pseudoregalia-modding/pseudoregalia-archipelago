#pragma once

namespace Pseudoregalia_AP {
	enum class ItemType {
		Ability,
		HealthPiece,
		SmallKey,
		MajorKey,
		Unknown
	};

	class GameData {
	public:
		static int GetHealthPieces();
		static int GetSmallKeys();
		static bool* GetMajorKeys();
		static int ReceiveItem(int64_t);
	private:
		static ItemType GetItemType(int64_t);
		static int health_pieces;
		static int small_keys;
		static bool major_keys[5];
	};
}