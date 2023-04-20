#include "SoundPlayer.h"
#include <Windows.h>
#include <array>

#ifdef _DEBUG
#include <sstream>
#include "GraphicsEngine/Message/Console.h"
#endif // _DEBUG

namespace sound 
{
	SoundPlayer::SoundPlayer():
		m_IsMute(false),
		m_FileName("edm_electronic_demoscene_hardkick _ain_traveler_1"),
		m_Extension("mp3")
	{
		Initialize();
	}

	SoundPlayer::~SoundPlayer() {
		Release();
	}

	bool SoundPlayer::Initialize() {
		char ExePath[256];
		GetModuleFileName(NULL, ExePath, 256);
		std::string ExeDir = GetExeDir(ExePath);

		std::string AudioPath = "\"" + ExeDir + "\\" + m_FileName + "." + m_Extension + "\"";
		std::string cmd = "open " + AudioPath + " alias " + m_Extension;
		
		std::array<char, MAXERRORLENGTH> errorString;
		mciGetErrorStringA(
			mciSendStringA(
				cmd.c_str(),
				nullptr,
				0,
				nullptr),
			errorString.data(),
			MAXERRORLENGTH);
		//Console::Log(">>>>>>>>>>>>>>>>>>[Audio Error Log] %s / AudioPath: %s\n", errorString.data(), AudioPath.c_str());

		return true;
	}

	bool SoundPlayer::Play() 
	{
		if (m_IsMute) return Pause();

		std::string cmd = "play " + m_Extension;
		std::array<char, MAXERRORLENGTH> errorString;
		mciGetErrorStringA(
			mciSendStringA(cmd.c_str(), nullptr, 0, nullptr),
			errorString.data(),
			MAXERRORLENGTH);
		//Console::Log(">>>>>>>>>>>>>>>>>>[Audio Error Log] %s\n", errorString.data());
		return true;
	}

	bool SoundPlayer::Pause() {
		std::string cmd = "pause " + m_Extension;
		mciSendStringA(cmd.c_str(), NULL, 0, NULL);

		return true;
	}

#ifdef _DEBUG
	void SoundPlayer::Skip(float SkipOffset)
	{
		float Offset = (SkipOffset * 1000.0f); // ミリ秒に直す

		std::ostringstream ss;
		ss << Offset;
		std::string SkipOffset_str(ss.str());
		std::string cmd = "seek " + m_Extension + " to " + SkipOffset_str;

		std::array<char, MAXERRORLENGTH> errorString;
		mciGetErrorStringA(
			mciSendStringA(cmd.c_str(), NULL, 0, NULL),
			errorString.data(),
			MAXERRORLENGTH);

		// スキップした後は、もう一度Playしないと音が鳴らない
		Play();
	}

	void SoundPlayer::Mute(bool IsMute)
	{
		m_IsMute = IsMute;
	}
#endif // _DEBUG


	void SoundPlayer::Release() {
		std::string cmd = "close " + m_Extension;
		mciSendStringA(cmd.c_str(), NULL, 0, NULL);
	}

	std::string SoundPlayer::GetExeDir(char path[]) {
		std::string path_str(path);
		int pathBlockOrder=path_str.rfind("\\");
		std::string ExeDir = path_str.erase(pathBlockOrder);
		return ExeDir;
	}
}
