#pragma once

#include <AsciirHeaders.h>

namespace Asciir
{
namespace Tools
{
	/// @brief a Asciir layer that provides an interface when the application is running, for the ChrTrcProfiler implemented into the Asciir library
	/// 
	/// this should be added as an overlay to the application.
	/// to toggle profiling, use the shortcut ctrl + P, this will begin a profiling session, until ctrl + P is hit again, the buffer is filled, or the timeout is hit (if specified).
	/// 
	/// @attention There should never be more than one ProfilingLayer active at a time, otherwise behaviour of when the session is active will be undefined.
	/// 
	class ProfilingLayer : public Layer
	{
	public:
		/// @brief constructs a profiling layer with the specified profiling buffer size and timeout.
		/// @param buffer_size size of the CTProfiler buffer.
		/// @param timeout timeout for each profiling session (-1 = no timeout).
		/// @param out_dir output file for the profile result file
		ProfilingLayer(size_t buffer_size, bool auto_start, DeltaTime timeout = -1, std::filesystem::path out_dir = "Profile.json")
			: m_buffer_size(buffer_size), m_timeout(timeout), m_out_dir(out_dir)
		{
			ChrTrcProfiler::CTProfiler::beginSession(buffer_size, m_out_dir, true, (duration)m_timeout);
		}

		void onAdd() override { AR_CORE_INFO("Added ProfilingLayer to the application"); }

		/// @brief starts / stops a profiling session, if ctrl + P is hit.
		void onUpdate(DeltaTime) override;

		/// @brief on layer remove, end a profiling session, if active.
		void onRemove() override;

	protected:

		size_t m_buffer_size;
		DeltaTime m_timeout;
		std::filesystem::path m_out_dir;

	};
}
}
