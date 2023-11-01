#include <string>
#include <random>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

namespace DeathLinkMessages {
	// Private members
	namespace {
		// Messages that other players see when you die.
		// These are wstrings because they'll never go out to other games and I like the heart emoji.
		const std::vector<std::wstring> own_deathlink_messages = {
			L"Sending death to your friends ♥",
			L"Are you sure that jump is possible?",
		};

		// Messages that you see when you die.
		const std::vector<std::string> outgoing_deathlink_messages = {
			"{} slipped and fell.",
			"{} probably died to an egg.",
		};

		const int own_messages_max = own_deathlink_messages.size() - 1;
		const int outgoing_messages_max = outgoing_deathlink_messages.size() - 1;
		const boost::random::mt19937 gen;
	}

	// Returns a random death message for the player who died.
	std::wstring RandomOwnDeathlink() {
		boost::random::uniform_int_distribution<> dist(0, own_messages_max);
		return own_deathlink_messages[dist(gen)];
	}

	// Returns a random outgoing death link message.
	std::string RandomOutgoingDeathlink() {
		boost::random::uniform_int_distribution<> dist(0, outgoing_messages_max);
		return outgoing_deathlink_messages[dist(gen)];
	}
}