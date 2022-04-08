#include "Texture.h"

namespace Asciir
{
	// ============ ShaderSequence ============

	template<typename TShader>
	ShaderSequence<TShader, enable_if_shader<TShader>>::ShaderSequence(std::initializer_list<Ref<TShader>> init_values)
		: m_frames(init_values) {}

	template<typename TShader>
	ShaderSequence<TShader, enable_if_shader<TShader>>::ShaderSequence(const ShaderSequence<TShader>& other)
		: m_curr_frame(other.m_curr_frame), m_frames(other.m_frames) {}

	template<typename TShader>
	Ref<TShader> ShaderSequence<TShader, enable_if_shader<TShader>>::incrFrame(size_t jump_size)
	{
		m_curr_frame += jump_size;
		m_curr_frame %= m_frames.size();

		return m_frames[m_curr_frame];
	}

	template<typename TShader>
	Ref<TShader> ShaderSequence<TShader, enable_if_shader<TShader>>::decrFrame(size_t jump_size)
	{
		// special case if the jump size is greater than the number of frames to the left of the current frame
		if (jump_size > m_frames.size() - m_curr_frame)
			m_curr_frame = m_frames.size() - jump_size - m_curr_frame;
		else
			m_curr_frame -= jump_size;

		return m_frames[m_curr_frame];
	}

	template<typename TShader>
	void ShaderSequence<TShader, enable_if_shader<TShader>>::addFrame(Ref<TShader> new_frame, size_t pos)
	{
		AR_ASSERT(pos < m_frames.size());

		// a frame will be added behind the current frame, thus the current frame will be shifted to the right.
		if (pos < m_curr_frame)
			m_curr_frame++;

		m_frames.insert(m_frames.begin() + pos, new_frame);
	}

	template<typename TShader>
	void ShaderSequence<TShader, enable_if_shader<TShader>>::addFrames(const std::vector<Ref<TShader>>& new_frames)
	{
		m_frames.insert(m_frames.end(), new_frames.begin(), new_frames.end());
	}

	template<typename TShader>
	void ShaderSequence<TShader, enable_if_shader<TShader>>::addFrames(const std::vector<Ref<TShader>>& new_frames, size_t pos)
	{
		AR_ASSERT(pos < frameCount());

		m_frames.insert(m_frames.begin() + pos, new_frames.begin(), new_frames.end());

		// the active frame should be shifted to the left by the amount of new frames added to the left of the active frame.
		if (pos < m_curr_frame)
			m_curr_frame += new_frames.size();
	}

	template<typename TShader>
	void ShaderSequence<TShader, enable_if_shader<TShader>>::removeFrame(size_t pos)
	{
		AR_ASSERT(pos < frameCount())
			m_frames.erase(m_frames.begin() + pos);

		// if the removed frame is to the left of the current frame, the current frame must be shifted to the left.
		if (pos <= m_curr_frame && m_curr_frame != 0)
			m_curr_frame--;
	}

	template<typename TShader>
	void ShaderSequence<TShader, enable_if_shader<TShader>>::removeFrames(size_t start, size_t end)
	{
		AR_ASSERT(start < end);
		AR_ASSERT(end < frameCount());

		m_frames.erase(m_frames.begin() + start, m_frames.begin() + end);

		// in case the current frame is erased, choose the closest frame to the left of it, as the next active frame
		if (m_curr_frame > start && m_curr_frame < end)
			m_curr_frame = start;
		// the current frame should be shifted to the left by the amount of frames removed
		else if (start < m_curr_frame)
			m_curr_frame -= start - end;
	}
}
