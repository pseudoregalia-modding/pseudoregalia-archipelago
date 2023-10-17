#pragma once

namespace Client {
	void Connect(const char*, const char*, const char*);
	void SendCheck(int64_t, std::wstring);
	void PollServer();
	void CompleteGame();
	void SendDeathLink();
}