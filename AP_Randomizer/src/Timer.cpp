#include <thread>
#include <functional>
#include "Timer.hpp"

namespace Timer {
	// Sets a timer (in milliseconds) and executes the function passed in when it expires.
	void Timer::CallbackAfterTimer(milliseconds delay, std::function<void()> callback) {
		auto run_timer = [delay, callback]() {
			std::this_thread::sleep_for(milliseconds(delay));
			callback();
			};

		std::thread timer_thread(run_timer);
	}
}