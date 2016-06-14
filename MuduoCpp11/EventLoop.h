#pragma once
#include "Common.h"
#include <functional>

namespace xtProximaCentauri
{
	class EventLoop
	{
		NoCopy(EventLoop);
		typedef std::function<void()> Functor;
	public:
		EventLoop();
		~EventLoop();
	};
}

