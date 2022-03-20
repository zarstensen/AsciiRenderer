#pragma once

#include <SFML/Audio.hpp>

#include "Asciir/Core/Timing.h"
#include "Asciir/Logging/Log.h"
#include "Asciir/Maths/Vector.h"

namespace Asciir
{
	/// @brief small class storing the path to an audio file, as well as how it should be loaded.
	///
	/// if toggleFX is set to true, the audio file will be loaded into memory, when an AudioPlayer loads it.
	/// Otherwise, it will be read directly from disk.
	/// 
	/// the audio file can be one of these formats:
	///		WAV
	///		OGG
	///		FLAC
	/// 
	/// the AudioSource is not required to exists whilst the AudioPlayer has the sound loaded. It is only required on the load function.
	/// this also means any subsequent changes to the AudioSource will not be reflected in the AudioPlayer, unless it is reloaded.
	/// 
	class AudioSource
	{

		friend class AudioPlayer;

	public:

		/// @brief constructs an AudioSource object
		/// @param audio_file path to the audio file ("" = empty AudioSource)
		/// @param is_fx wether it should be loaded as an effect file (in memory).
		AudioSource(std::filesystem::path audio_file = "", bool is_fx = false)
			: m_is_fx(is_fx)
		{
			setFile(audio_file);
		}

		/// @brief sets the fiel the audio should be loaded from.
		/// 
		/// the file should be a valid format, otherwise behaviour is undefined.
		void setFile(std::filesystem::path audio_file)
		{
			// TODO: check if audio_file is a valid audio format
			m_audio_file = audio_file;

			// do not reuse fx buffer, as other AudioPlayer's might need it.
			m_fx_buffer = nullptr;
		}

		/// @brief sets wether the audio file should be loaded into memory. 
		void setFX(bool is_fx);
		bool isFX() const { return m_is_fx; }

		const std::filesystem::path& getFile() const { return m_audio_file; }

		Ref<sf::SoundBuffer> getFXBuff() { return m_fx_buffer; }

	protected:

		/// @brief sets up the m_fx_buffer, if needed
		void setupFX();

		std::filesystem::path m_audio_file;
		bool m_is_fx = false;
		Ref<sf::SoundBuffer> m_fx_buffer;
	};

	typedef sf::SoundStream::Chunk Chunk;

	/// @brief abstract class for a generative audio source.  
	/// this should be used to generate audio at runtime. If this is not needed, use AudioSource instead.
	/// 
	/// override onLoad, genAudio, channels, and sampleRate to fully implement this class.
	/// 
	class AudioGenSrc
	{
	public:
		virtual ~AudioGenSrc() {}

		/// @brief makes sure the AudioGenSrc is able to supply audio data.
		/// @return wether the function was successfull
		virtual bool onLoad() { return true; };

		/// @brief generates the next chunk of audio data.  
		/// 
		/// the audio data should be stored in the data parameter passed.
		/// if the function returns false, the AudioPlayer which has this source loaded, will stop.
		/// 
		/// @param data_out refrence to store the result in
		virtual bool genAudio(Chunk& data_out) = 0;
		/// @brief seeks the passed offset in the audio data, relative to the start.
		virtual void seek(DeltaTime offset) = 0;

		/// @return the channel count for the generated audio data
		virtual uint16_t channels() = 0;
		/// @return the sample rate for the generated audio data
		virtual uint32_t sampleRate() = 0;
	};

	typedef sf::SoundSource::Status Status;

	class AudioPlayer
	{
	public:
		AudioPlayer() = default;
		AudioPlayer(AudioSource& src) { load(src); }
		AudioPlayer(AudioGenSrc& src) { load(src); }

		~AudioPlayer() { if (loaded()) unload(); }

		void load(AudioSource& src);
		void load(AudioGenSrc& src);

		void unload();

		void reload(AudioSource& src) { unload(); load(src); }
		void reload(AudioGenSrc& src) { unload(); load(src); }

		bool loaded() const { return m_has_loaded; }

		/// @brief plays / resumes the audio
		void play() { getSrc().play(); }
		/// @brief stops the audio
		void stop() { getSrc().stop(); }
		/// @brief pauses the playing of the audio
		void pause() { getSrc().pause(); }

		Status getStatus() { return getSrc().getStatus(); }

		/// @brief sets how quickly the audio will decrease in volume, when moving away from the listener.
		void setAttenuation(float attenuation = 1) { getSrc().setAttenuation(attenuation); }
		/// @brief makes the AudioPlayer play the audio source at the same volume, no matter its position relative to the listener.  
		/// same as setAttenuation(0);
		void makeGlobal() { setAttenuation(0); }

		/// @brief sets the position of the AudioPlayer.
		/// @note due to the SFML library using floats as their real data type,
		/// if Asciir was built with high precission real, it will have no effect here, and still be converted to a flaot.
		void setPosition(Coord pos) { getSrc().setPosition(sf::Vector3f(pos.x, 0, pos.y)); }
		/// @brief gets the position of the AudioPlayer.
		/// @note see setPosition note
		Coord getPosition();

		/// @brief gives the SFML SoundSource object responsible for playing the audio.
		/// use this function to acces any properties that are not exposed by the AudioPlayer
		sf::SoundSource& getSrc() { return *m_audio_src; };

	protected:
		sf::SoundSource* m_audio_src = nullptr;
		Ref<sf::SoundBuffer> m_fx_buff;
		bool m_has_loaded = false;
	};

}