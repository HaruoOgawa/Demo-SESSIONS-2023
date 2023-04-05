#pragma once
#include <vector>
#include <string>

namespace sound 
{
	class SoundPlayer
	{
		bool m_IsMute;
		std::string m_FileName;
		std::string m_Extension;
	public:
		SoundPlayer();
		virtual ~SoundPlayer();
		bool Initialize();
		bool Play();
		bool Pause();
		void Release();
		void Skip(float SkipOffset);
		void Mute(bool IsMute);

	private:
		std::string GetExeDir(char path[]);
	};
}