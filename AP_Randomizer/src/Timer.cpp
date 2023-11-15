#pragma once
#include <thread>
#include <functional>
#include <mutex>
#include "Timer.hpp"
#include "Logger.hpp"

namespace Timer {
	namespace {
		struct RunningTimer {
			float remaining_time;
			bool* locked_boolean;
			std::function<void()> callback;
		};
		std::mutex timer_mutex;
		std::vector<RunningTimer> active_timers;
	}
	using std::chrono::milliseconds;

	void Timer::RunTimerRealTime(milliseconds delay, std::function<void()> callback) {
		auto run_timer = [delay, callback]() {
			std::this_thread::sleep_for(milliseconds(delay));
			callback();
			};

		std::thread timer_thread(run_timer);
		timer_thread.detach();
	}

	// Sets the input boolean to true, sets a timer, and sets the boolean back to false when it expires.
	void Timer::RunTimerRealTime(milliseconds delay, bool* lock) {
		auto run_timer = [delay, lock]() {
			*lock = true;
			std::this_thread::sleep_for(delay);
			*lock = false;
			};

		std::thread locker_thread(run_timer);
		locker_thread.detach();
	}

	void Timer::RunTimerInGame(float timer_max, bool* to_lock) {
		*to_lock = true;
		RunningTimer new_timer{ timer_max, to_lock, nullptr };
		Log(L"Setting timer for " + std::to_wstring(timer_max));
		std::lock_guard<std::mutex> guard(timer_mutex);
		active_timers.push_back(new_timer);
	}

	void Timer::RunTimerInGame(float timer_max, std::function<void()> callback) {
		RunningTimer new_timer{ timer_max, nullptr, callback };
		Log(L"Setting timer for " + std::to_wstring(timer_max));
		std::lock_guard<std::mutex> guard(timer_mutex);
		active_timers.push_back(new_timer);
	}

	// Decrements all active timer and executes relevant operations for each one that expires.
	void Timer::OnTick(float delta_seconds) {
		// TODO: this is pretty hard to read

		std::vector<RunningTimer>::iterator timer;
		std::lock_guard<std::mutex> guard(timer_mutex);
		for (timer = active_timers.begin(); timer != active_timers.end(); ) {
			timer->remaining_time -= delta_seconds;
			if (timer->remaining_time <= 0.0f) {
				if (timer->locked_boolean != nullptr) {
					*(timer->locked_boolean) = false;
				}
				if (timer->callback != nullptr) {
					timer->callback();
				}
				timer = active_timers.erase(timer);
			}
			else {
				timer++;
			}
		}
	}
}