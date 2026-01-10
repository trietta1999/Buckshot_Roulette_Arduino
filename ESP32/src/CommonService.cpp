/**
 * @brief Common service library
 */

#include "CommonService.h"
#include "CommonData.h"
#include "CommonLibrary.h"
#ifdef _WIN64
#include <iostream>
#include <iomanip>
#include <thread>
#include <sstream>
#else
#include <esp_random.h>
#include "../Sound.h"
#endif
#include "SampleSound.h"

#ifdef _WIN64
WAVEFORMATEX wfx = {};
HWAVEOUT hWaveOut;

std::vector<std::string> dummyPlaylist = {
    "Music_1",
    "Music_2",
    "Music_3",
    "Music_4",
    "Music_5",
    "Music_6",
    "Music_7",
    "Music_8",
    "Music_9",
    "Music_10",
};
#endif

#ifdef _WIN64
static void PlayRawPCM(const uint8_t* pcmData, uint32_t size)
{
    MMRESULT result;

    // Prepare header
    WAVEHDR whdr = {};
    whdr.lpData = (LPSTR)pcmData;
    whdr.dwBufferLength = (DWORD)size;
    whdr.dwFlags = 0;

    // Prepare cache
    result = waveOutPrepareHeader(hWaveOut, &whdr, sizeof(WAVEHDR));
    if (result != MMSYSERR_NOERROR)
    {
        waveOutClose(hWaveOut);
        return;
    }

    // Write sound data
    result = waveOutWrite(hWaveOut, &whdr, sizeof(WAVEHDR));
    if (result != MMSYSERR_NOERROR)
    {
        waveOutUnprepareHeader(hWaveOut, &whdr, sizeof(WAVEHDR));
        waveOutClose(hWaveOut);
        return;
    }

    // Wait until playback is complete
    // Check the WHDR_DONE flag. This is a bit resource-intensive because it uses a loop,
    // but it's the simplest way.
    while (!(whdr.dwFlags & WHDR_DONE))
    {
        Sleep(10);
    }

    // Free header
    waveOutUnprepareHeader(hWaveOut, &whdr, sizeof(WAVEHDR));
}

static void AttachConsoleWindow()
{
    AllocConsole();
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);
    freopen_s(&fp, "CONIN$", "r", stdin);
}

static void DebugConsoleProcess()
{
    // Debug console
    if (debug_data::InputParamList.GetState())
    {
        try {
            auto inputParams = debug_data::InputParamList.GetValue();

            // Test show message box
            if (inputParams.at(0) == "test")
            {
                ::MessageBox(NULL, L"Test show message box", L"Test", MB_OK);
            }
            // Transit state
            else if (inputParams.at(0) == "transit")
            {
                std::string state = inputParams.at(1);
                std::transform(state.begin(), state.end(), state.begin(), ::toupper);

                auto resultStateInfo = std::find_if(map_STATE_TYPE.begin(), map_STATE_TYPE.end(),
                    [state](const std::pair<STATE_TYPE, std::string>& item) {
                        return (item.second == state);
                    });

                CurrentState.SetValue(resultStateInfo->first);
            }
            // Change player
            else if (inputParams.at(0) == "player")
            {
                std::string player = inputParams.at(0) + inputParams.at(1);
                std::transform(player.begin(), player.end(), player.begin(), ::toupper);

                auto resultPlayerInfo = std::find_if(map_PLAYER_TYPE.begin(), map_PLAYER_TYPE.end(),
                    [player](const std::pair<PLAYER_TYPE, std::string>& item) {
                        return (item.second == player);
                    });

                debug_data::CurrentPlayer.SetValue(resultPlayerInfo->first);
            }
            // Change player table preset
            else if (inputParams.at(0) == "setitem")
            {
                debug_data::CurrentItemType.SetValue((ITEM_TYPE)std::stoi(inputParams.at(1)));
            }
            // Change player HP
            else if (inputParams.at(0) == "sethp")
            {
                std::string player = inputParams.at(1);
                uint8_t level = std::stoi(inputParams.at(2));
                uint8_t hp = std::stoi(inputParams.at(3));

                std::transform(player.begin(), player.end(), player.begin(), ::toupper);

                auto resultPlayerInfo = std::find_if(map_PLAYER_TYPE.begin(), map_PLAYER_TYPE.end(),
                    [player](const std::pair<PLAYER_TYPE, std::string>& item) {
                        return (item.second == player);
                    });

                debug_data::PlayerHP.SetValue(std::make_tuple(resultPlayerInfo->first, level, hp));
            }
            // Play sound
            else if (inputParams.at(0) == "sound")
            {
                auto type = std::stoi(inputParams.at(1));
                PlaySoundWrapper((SOUND_TYPE)type);
            }
            // Music EOF
            else if (inputParams.at(0) == "musiceof")
            {
                MusicState.SetValue(MUSIC_STATE_TYPE::MSG_EOF);
            }
            // Special command
            else
            {
                debug_data::SpecialCommand.SetValue(inputParams.at(0));
            }

            debug_println("Process debug data done!");
        }
        catch (...)
        {
            debug_println("Process debug data fail! Try again!");
        }

        debug_data::InputParamList.SetValue({ });
        debug_data::InputParamList.ResetState();
    }
}
#endif

static void PlaySampleSound(const uint8_t* sample, uint32_t size)
{
#ifdef _WIN64
    PlayRawPCM(sample, size);
#else
    PlaySoundFromSample(sample, size);
#endif
}

void CommonBeep(uint16_t frequency, uint16_t duration)
{

}

void InitData()
{
#ifndef _WIN64
    uint32_t seed = esp_random();

    SetupSound();
#else
    uint32_t seed = time(0);
#endif

    // Set random seed
    srand(seed);

#ifdef _WIN64
    AttachConsoleWindow();

    // Config format (WAVEFORMATEX)
    wfx.wFormatTag = WAVE_FORMAT_PCM;
    wfx.nChannels = 1; // Mono chanel
    wfx.nSamplesPerSec = 44100;
    wfx.wBitsPerSample = 8; // Bit/sample: 8-bit
    wfx.nBlockAlign = wfx.nChannels * (wfx.wBitsPerSample / 8); // Block size (bytes)
    wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign; // Data rate (bytes/sec)
    wfx.cbSize = 0;

    // Open sound device
    MMRESULT result = waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    if (result != MMSYSERR_NOERROR)
    {
        debug_println_func("Open sound device fail!");
    }
#endif
}

void CommonServiceProcess()
{
#ifdef _WIN64
    std::thread([] {
        std::string inputConsole;

        try
        {
            std::getline(std::cin, inputConsole);

            if (!inputConsole.empty())
            {
                std::istringstream iss(inputConsole);
                std::string param;
                std::vector<std::string> inputParams;

                while (iss >> param) {
                    inputParams.push_back(param);
                }

                debug_data::InputParamList.SetValue(inputParams);

                DebugConsoleProcess();
            }
        }
        catch (...)
        {
            debug_println("Process debug data fail! Try again!");
        }
        }).detach();
#else
    // Read data from serial
    if (Serial.available()) {
        String read = Serial.readStringUntil('\n');
    }

    RunMusicTask();
#endif
}

void PlaySoundWrapper(SOUND_TYPE type)
{
    if (SoundEnable.GetValue())
    {
        switch (type) {
        case SOUND_TYPE::SHOTGUN_SHOT:
            PlaySampleSound(shotgun_sound, _countof(shotgun_sound));
            break;
        case SOUND_TYPE::LOAD_SHELL:
            PlaySampleSound(load_shell_sound, _countof(load_shell_sound));
            break;
        case SOUND_TYPE::DROP:
            PlaySampleSound(shotgun_drop_sound, _countof(shotgun_drop_sound));
            break;
        case SOUND_TYPE::PICK:
            PlaySampleSound(pick_sound, _countof(pick_sound));
            break;
        case SOUND_TYPE::ASSIGN:
            PlaySampleSound(assign_sound, _countof(assign_sound));
            break;
        case SOUND_TYPE::HIT:
            PlaySampleSound(hit_sound, _countof(hit_sound));
            break;
        case SOUND_TYPE::WEAPON_PICKUP:
            PlaySampleSound(weapon_pickup_sound, _countof(weapon_pickup_sound));
            break;
        case SOUND_TYPE::BREAK:
            PlaySampleSound(break_sound, _countof(break_sound));
            break;
        case SOUND_TYPE::HEALTH:
            PlaySampleSound(health_sound, _countof(health_sound));
            break;
        case SOUND_TYPE::ADRENALINE:
            PlaySampleSound(adrenaline_sound, _countof(adrenaline_sound));
            break;
        default:
            PlaySampleSound(null_sound, _countof(null_sound));
            break;
        }
    }
}

std::vector<std::string> ExportListMusicFilesWrapper()
{
#ifdef _WIN64
    return dummyPlaylist;
#else
    return ExportListMusicFiles();
#endif
}

void PlayMusicWrapper(std::string name)
{
#ifndef _WIN64
    audio.stopSong();
    PlayTrack(name.c_str());
#endif
}

void SetMusicStateWrapper(MUSIC_STATE_TYPE state)
{
#ifndef _WIN64
    switch (state)
    {
    case MUSIC_STATE_TYPE::RESUME:
        if (!audio.isRunning())
        {
            audio.pauseResume();
        }
        break;
    case MUSIC_STATE_TYPE::PAUSE:
        if (audio.isRunning())
        {
            audio.pauseResume();
        }
        break;
    case MUSIC_STATE_TYPE::STOP:
        audio.stopSong();
        break;
    default:
        break;
    }
#endif

    MusicState.SetValue(state);
}

uint8_t GetMusicMaxVolumeWrapper()
{
#ifdef _WIN64
    return 0;
#else
    return audio.maxVolume();
#endif
}

void SetMusicVolumeWrapper(uint8_t vol)
{
#ifndef _WIN64
    audio.setVolume(vol);
#endif
}

uint8_t GetCurrentMusicPercentWrapper()
{
#ifndef _WIN64
    uint8_t percent = 0;

    if (audio.getAudioFileDuration())
    {
        percent = audio.getAudioCurrentTime() / 2 * 100 / audio.getAudioFileDuration();
        if (percent > 100)
        {
            percent = 100;
        }
    }

    CurrentMusicPercent.SetValue(percent);

    return percent;
#endif
    return 0;
}
