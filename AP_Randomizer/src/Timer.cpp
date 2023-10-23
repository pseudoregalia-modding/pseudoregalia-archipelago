#pragma once
#include <thread>
#include <functional>
#include "Timer.hpp"

namespace Timer {
	// Sets a timer and executes the function passed in when it expires.
	void Timer::CallbackAfterTimer(milliseconds delay, std::function<void()> callback) {
		auto run_timer = [delay, callback]() {
			std::this_thread::sleep_for(milliseconds(delay));
			callback();
			};

		std::thread timer_thread(run_timer);
		timer_thread.detach();
	}

	// Sets the input boolean to true, sets a timer, and sets the boolean back to false when it expires.
	void Timer::SetBooleanAfterTimer(milliseconds delay, bool* lock) {
		auto run_timer = [delay, lock]() {
			*lock = true;
			std::this_thread::sleep_for(delay);
			*lock = false;
			};

		std::thread locker_thread(run_timer);
		locker_thread.detach();
	}
}