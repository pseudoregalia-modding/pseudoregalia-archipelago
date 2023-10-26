namespace Timer {
	using std::chrono::milliseconds;
	void RunTimerRealTime(std::chrono::milliseconds, std::function<void()>);
	void RunTimerRealTime(std::chrono::milliseconds, bool*);
	void RunTimerInGame(float, bool*);
	void RunTimerInGame(float, std::function<void()>);
	void OnTick(float);
}