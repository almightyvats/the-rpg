#ifndef RPG_TIMER_HPP
#define RPG_TIMER_HPP
// https : // github.com/99x/timercpp

#include <chrono>
#include <functional>
#include <iostream>
#include <thread>

using LambdaFunction = std::function<void(void)>;

class RpgTimer {
	bool m_clear;

  public:
	RpgTimer() { m_clear = false; }
	~RpgTimer() {}
	void setTimeout(LambdaFunction function, int delay);
	void setInterval(LambdaFunction function, int interval);
	void stop();
};

#endif