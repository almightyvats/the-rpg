#include "RpgTimer.hpp"

void RpgTimer::setTimeout(LambdaFunction function, int delay)
{
	m_clear = false;
	std::thread t([=]() {
		if (m_clear)
			return;
		std::this_thread::sleep_for(std::chrono::milliseconds(delay));
		if (m_clear)
			return;
		function();
	});
	t.detach();
}

void RpgTimer::setInterval(LambdaFunction function, int interval)
{
	m_clear = false;
	std::thread t([=]() {
		while (true) {
			if (m_clear)
				return;
			std::this_thread::sleep_for(std::chrono::milliseconds(interval));
			if (m_clear)
				return;
			function();
		}
	});
	t.detach();
}

void RpgTimer::stop()
{
	m_clear = true;
}