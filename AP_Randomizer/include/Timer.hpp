namespace Timer {
	using std::chrono::milliseconds;
	void CallbackAfterTimer(std::chrono::milliseconds, std::function<void()>);
}