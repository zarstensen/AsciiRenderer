#include "Audio.h"

#include "Asciir/Logging/Log.h"

namespace Asciir
{
	void AudioSource::setFX(bool is_fx)
	{
		if (is_fx && !m_is_fx)
		{
			m_is_fx = true;
		}
		else if (!is_fx && m_is_fx)
		{
			m_is_fx = false;
			m_fx_buffer = nullptr;
		}
	}

	void AudioSource::setupFX()
	{
		if (!m_fx_buffer)
		{
			m_fx_buffer.allocate();
			AR_VERIFY_MSG(m_fx_buffer->loadFromFile(m_audio_file), "Failed to load audio file: ", m_audio_file);
		}
	}
	
	// ============ AudioPlayer ============
	
	void AudioPlayer::load(AudioSource& src)
	{
		if (src.isFX())
		{
			src.setupFX();
			m_fx_buff = src.getFXBuff();
			m_audio_src = new sf::Sound(*m_fx_buff);
		}
		else
		{
			m_audio_src = new sf::Music;
			AR_VERIFY_MSG(((sf::Music*)m_audio_src)->openFromFile(src.getFile()), "Failed to load audio file: ", src.getFile());
		}

		m_has_loaded = true;
	}

	void AudioPlayer::unload()
	{
		AR_ASSERT_MSG(m_has_loaded, "Cannot unload Audio Source if it was not loaded in the first place.");

		delete m_audio_src;
		m_fx_buff = nullptr;
	}

	Coord AudioPlayer::getPosition()
	{
		sf::Vector3f pos = getSrc().getPosition();
		return Coord(pos.x, pos.z);
	}
	
}
