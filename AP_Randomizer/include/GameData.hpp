#pragma once

namespace Pseudoregalia_AP {
	class GameData {
	public:
		static int GetHealthPieces();
		static int GetSmallKeys();
		static bool* GetMajorKeys();
	private:
		static int health_pieces;
		static int small_keys;
		static bool major_keys[5];
	};
}