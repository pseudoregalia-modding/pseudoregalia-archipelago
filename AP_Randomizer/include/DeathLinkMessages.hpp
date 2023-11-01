#include <string>
#include <random>
#include <format>
#include <ctime>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

namespace DeathLinkMessages {
	/*
	Some messages are based on or inspired by suggestions from others.
	These come with an in-line comment mentioning the name of the person who suggested the message.
	*/
	namespace {
		// Messages that other players see when you die.
		// These are wstrings because they'll never go out to other games and I like the heart emoji.
		const std::vector<std::wstring> own_deathlink_messages = {
			L"Sending death to your friends ♥",
			L"Are you sure that jump is possible?",
			L"Tip: You can lower the logic level in your yaml settings.",
			L"Maybe that check isn't so important?", // Garbo
			L"Maybe you should look for an alternate route.",
		};

		// Messages that you see when you die.
		const std::vector<std::string> outgoing_deathlink_messages = {
			"{} slipped and fell.",
			"{} probably died to an egg.",
			"{}'s controller came unplugged.",
			"{} fell asleep.",
			"{} needs more health pieces.",
			"{}'s disguise didn't work.",
			"{} might need to look for an alternate route.", // Garbo
			"{} got in a fight with gravity and lost.", // Garbo
			"{} didn't schmoove hard enough.", // MuffinJets
		};

		const int own_messages_max = own_deathlink_messages.size() - 1;
		const int outgoing_messages_max = outgoing_deathlink_messages.size() - 1;
		boost::random::mt19937 gen(time(0));
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