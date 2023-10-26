namespace Timer {
	using std::chrono::milliseconds;
	void RunTimerRealTime(std::chrono::milliseconds, std::function<void()>);
	void RunTimerRealTime(std::chrono::milliseconds, bool*);
}