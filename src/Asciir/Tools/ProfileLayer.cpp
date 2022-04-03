#include "ProfileLayer.h"

#include <ChrTrc.h>

using namespace ChrTrcProfiler;

namespace Asciir
{
namespace Tools
{
	void ProfilingLayer::onUpdate(DeltaTime)
	{
		if (Input::isKeyDown(Key::LEFT_CONTROL) && Input::isKeyToggled(Key::P))
		{
			if (CTProfiler::hasSession())
			{
				AR_CORE_NOTIFY("ProfilingLayer: Stopped Profiling!");
				CTProfiler::endSession();
			}
			else
			{
				AR_CORE_NOTIFY("ProfilingLayer: Begun Profiling!");
				CTProfiler::beginSession(m_buffer_size, m_out_dir, true, (duration)m_timeout);
			}
		}
	}

	void ProfilingLayer::onRemove()
	{
		CTProfiler::endSession();
	}
}
}
