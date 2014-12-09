#ifndef _RTS_SOUND_
#define _RTS_SOUND_

#include <vector>
#include <dshow.h>
#include <d3dx9.h>
#include <dsound.h>
#include "debug.h"

/*		
// The DirectMusic API has been deprecated in rescent
// DirectX releases. Use XAudio2 library instead.
// Alternativly, download and use 3rd party sound library like FMOD (http://www.fmod.org/)

class SOUND;

class SOUNDFILE
{
	friend class SOUND;
	public:
		SOUNDFILE();
		~SOUNDFILE();
		void Load(WCHAR fileName[], SOUND &sound);

	private:
		IDirectMusicSegment8 *m_pSegment;
};

class SOUND
{
	friend class SOUNDFILE;
	public:
		SOUND();
		~SOUND();
		void Init(HWND windowHandle);
		
		void PlaySound(int soundID, bool loop);
		void SetMasterVolume(float volume);
		float GetMasterVolume(){return m_masterVolume;}

	private:
		IDirectMusicPerformance8 *m_pPerformance;
		IDirectMusicLoader8 *m_pLoader;

		float m_masterVolume;
		std::vector<SOUNDFILE*> m_sounds;
};
*/

class MP3{
	public:
		MP3();
		~MP3();
		void Release();
		void Init();
		void LoadSong(WCHAR fName[]);
		void Play();
		void Stop();
		bool IsPlaying();
		void SetVolume(float volume);
		void SetBalance(float balance);

	private:

		IGraphBuilder *m_pGraphBuilder;
		IMediaControl *m_pMediaControl;
		IMediaPosition *m_pMediaPosition;
		IBasicAudio *m_pBasicAudio;
};

#endif