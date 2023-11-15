#pragma once

namespace Client {
	void Connect(const std::string, const std::string, const std::string);
	void SendCheck(int64_t, std::wstring);
	void PollServer();
	void CompleteGame();
	void SendDeathLink();
	void Disconnect();
}