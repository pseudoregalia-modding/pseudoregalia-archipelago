namespace Timer {
	using std::chrono::milliseconds;
	void CallbackAfterTimer(std::chrono::milliseconds, std::function<void()>);
	void SetBooleanAfterTimer(std::chrono::milliseconds, bool*);
}