#include <string>
#include <random>
#include <format>
#include <ctime>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include "GameData.hpp"

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
			L"Maybe that check isn't so important?", // Garbo
			L"Maybe you should look for an alternate route.",
			L"Tip: You can lower the logic level in your yaml settings.",
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

		// TODO: figure out a system to make these work cleanly with flags.
		const std::vector<std::wstring> high_logic_messages = {
		};

		const int own_messages_max = own_deathlink_messages.size() - 1;
		const int outgoing_messages_max = outgoing_deathlink_messages.size() - 1;
		boost::random::mt19937 gen(time(0));
	}

	// Returns a random death message for the player who died.
	std::wstring RandomOwnDeathlink() {
		// TODO: potentially add situational death link messages like this
		/*
		using std::begin, std::end;
		std::vector<std::wstring> own_messages(own_deathlink_messages);
		if (GameData::GetOptions().at("logic_level") > 1) {
			own_messages.insert(end(own_messages), begin(high_logic_messages), end(high_logic_messages));
		}
		*/
		boost::random::uniform_int_distribution<> dist(0, own_messages_max);
		return own_deathlink_messages[dist(gen)];
	}

	// Returns a random outgoing death link message.
	std::string RandomOutgoingDeathlink() {
		boost::random::uniform_int_distribution<> dist(0, outgoing_messages_max);
		return outgoing_deathlink_messages[dist(gen)];
	}
}