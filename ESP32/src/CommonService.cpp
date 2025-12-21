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
#include "../Music.h"
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
#endif

static void PlaySampleSound(const uint8_t* sample, uint32_t size)
{
#ifdef _WIN64
    PlayRawPCM(sample, size);
#else
    sound::PlaySound(sample, size);
#endif
}

#ifdef _WIN64
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
                CommonPlaySound((SOUND_TYPE)type);
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

void CommonBeep(uint16_t frequency, uint16_t duration)
{
#ifdef _WIN64
    //::Beep(frequency, duration);
#else
    //HardwareBeep(frequency, duration);
#endif
}

void InitData()
{
#ifndef _WIN64
    uint32_t seed = esp_random();
#else
    uint32_t seed = time(0);
#endif

    // Set random seed
    srand(seed);

    // Create random init data
    //sys_host::RandomSeed.SetValue(seed);
    //sys_host::LabelIndicator.SetValue((LABEL_INDICATOR)RandomRange(0, (uint8_t)LABEL_INDICATOR::MAX));
    //sys_host::BatteryType.SetValue((BATTERY_TYPE)RandomRange(0, (uint8_t)BATTERY_TYPE::MAX));
    //sys_host::ComPortType.SetValue((COMPORT_TYPE)RandomRange(0, (uint8_t)COMPORT_TYPE::MAX));
    //sys_host::BatteryNum.SetValue(RandomRange(1, 5));
    //sys_host::SerialNum.SetValue(GenerateSerialNumber());
    //sys_host::EndlessTimeClock.SetValue(std::make_tuple(0, 0, 0));

    //sys_host::StrikeNum.SetValue(0);
    //sys_host::TimeCycle.SetValue(TIMECYCLE_0);
    //sys_gui::SuccessState.SetValue(INCORRECT);

#ifdef _WIN64
    AttachConsoleWindow();

    // Config format (WAVEFORMATEX)
    wfx.wFormatTag = WAVE_FORMAT_PCM;
    wfx.nChannels = 1; // Mono chanel
    wfx.nSamplesPerSec = SAMPLE_RATE;
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

    // Print init data
#ifdef _WIN64
    debug_println("===== Dummy Data Initialized =====");
    //debug_println("RandomSeed: " + std::to_string(sys_host::RandomSeed.GetValue()));
    //debug_println("LabelIndicator: " + map_LABEL_INDICATOR[sys_host::LabelIndicator.GetValue()]);
    //debug_println("BatteryType: " + map_BATTERY_TYPE[sys_host::BatteryType.GetValue()]);
    //debug_println("ComPortType: " + map_COMPORT_TYPE[sys_host::ComPortType.GetValue()]);
    //debug_println("BatteryNum: " + std::to_string(sys_host::BatteryNum.GetValue()));
    //debug_println("SerialNum: " + sys_host::SerialNum.GetValue());
    debug_println("==================================");
#endif
}

void CommonServiceProcess()
{
#ifndef UNIT_TEST
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

        // Print init data
        if (read == "sys_data") {
            //debug_println("===== Dummy Data Initialized =====");
            //debug_println("RandomSeed: " + std::to_string(sys_host::RandomSeed.GetValue()));
            //debug_println("LabelIndicator: " + map_LABEL_INDICATOR[sys_host::LabelIndicator.GetValue()]);
            //debug_println("BatteryType: " + map_BATTERY_TYPE[sys_host::BatteryType.GetValue()]);
            //debug_println("ComPortType: " + map_COMPORT_TYPE[sys_host::ComPortType.GetValue()]);
            //debug_println("BatteryNum: " + std::to_string(sys_host::BatteryNum.GetValue()));
            //debug_println("SerialNum: " + sys_host::SerialNum.GetValue());
            //debug_println("==================================");
        }
    }

    // Re-connect WiFi if disconnected
    //WiFiReconnect();

    // Handle web server
    //ServerHandleClient();

    music::RunMusicTask();
#endif
#endif
}

JsonDocument ProcessRequest(HWND hwnd, uint32_t msg, JsonDocument jsonDocIn)
{
    JsonDocument jsonDoc;

    return jsonDoc;
}

JsonDocument CommonSendRequest(uint32_t msg)
{
#ifdef _WIN64
    JsonDocument jsonDoc;

    jsonDoc["client_name"] = CLIENT_NAME_FOR_JSON;
    char jsonDocStr[MAX_SIZE] = { 0 };

    serializeJson(jsonDoc, jsonDocStr);

    HANDLE hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, BUFFER_SIZE, SHARED_MEM);
    if (!hMapFile)
    {
        return jsonDoc;
    }

    LPVOID pBuffer = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, BUFFER_SIZE);
    if (!pBuffer)
    {
        CloseHandle(hMapFile);
        return jsonDoc;
    }

    strcpy((char*)pBuffer, jsonDocStr);

    UnmapViewOfFile(pBuffer);

    HWND hwnd = ::FindWindow(NULL, HOST_NAME);

    ::SendMessage(hwnd, WM_SET_CLIENT_HANDLE, msg, NULL);
    ::SendMessage(hwnd, WM_REQUEST, msg, NULL);
#else
    //data_pack_t byteData = { 0 };
    //strcpy(byteData.source, CLIENT_NAME);
    //byteData.base_msg = WM_REQUEST;
    //byteData.msg = msg;

    //SendMessage(byteData);
#endif

    return JsonResponse.GetValue();
}

JsonDocument CommonSendRequestWithData(uint32_t msg, JsonDocument jsonValue)
{
#ifdef _WIN64
    char jsonDocStr[MAX_SIZE] = { 0 };
    jsonValue["client_name"] = CLIENT_NAME_FOR_JSON;

    serializeJson(jsonValue, jsonDocStr);

    HANDLE hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, BUFFER_SIZE, SHARED_MEM);
    if (!hMapFile)
    {
        return JsonDocument();
    }

    LPVOID pBuffer = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, BUFFER_SIZE);
    if (!pBuffer)
    {
        CloseHandle(hMapFile);
        return JsonDocument();
    }

    strcpy((char*)pBuffer, jsonDocStr);

    UnmapViewOfFile(pBuffer);

    HWND hwnd = ::FindWindow(NULL, HOST_NAME);

    ::SendMessage(hwnd, WM_SET_CLIENT_HANDLE, msg, NULL);
    ::SendMessage(hwnd, WM_REQUEST_WITH_DATA, msg, NULL);
#else
    //char jsonDocStr[MAX_SIZE] = { 0 };
    //serializeJson(jsonValue, jsonDocStr);

    //data_pack_t byteData = { 0 };
    //strcpy(byteData.source, CLIENT_NAME);
    //byteData.base_msg = WM_REQUEST_WITH_DATA;
    //byteData.msg = msg;
    //strcpy(byteData.data, jsonDocStr);

    //SendMessage(byteData);
#endif

    return JsonResponse.GetValue();
}

void SetupSound()
{
#ifndef _WIN64
    sound::Setup();
#endif
}

void UninstallSound()
{
#ifndef _WIN64
    sound::Uninstall();
#endif
}

void CommonPlaySound(SOUND_TYPE type)
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

void SetupMusic()
{
#ifndef _WIN64
    music::Setup();
#endif
}

void UninstallMusic()
{
#ifndef _WIN64
    music::Uninstall();
#endif
}

std::vector<std::string> CommonExportListMusicFiles()
{
#ifdef _WIN64
    return dummyPlaylist;
#else
    return music::ExportListMusicFiles();
#endif
}

void CommonPlayMusic(std::string name)
{
#ifndef _WIN64
    music::audio->stopSong();
    music::PlayTrack(name.c_str());
#endif
}

void SetMusicState(MUSIC_STATE_TYPE state)
{
#ifndef _WIN64
    switch (state)
    {
    case MUSIC_STATE_TYPE::RESUME:
        if (!music::audio->isRunning())
        {
            music::audio->pauseResume();
        }
        break;
    case MUSIC_STATE_TYPE::PAUSE:
        if (music::audio->isRunning())
        {
            music::audio->pauseResume();
        }
        break;
    case MUSIC_STATE_TYPE::STOP:
        music::audio->stopSong();
        break;
    default:
        break;
    }
#endif

    MusicState.SetValue(state);
}

uint8_t GetMusicMaxVolume()
{
#ifdef _WIN64
    return 0;
#else
    return music::audio->maxVolume();
#endif
}

void SetMusicVolume(uint8_t vol)
{
#ifndef _WIN64
    music::audio->setVolume(vol);
#endif
}

uint8_t GetCurrentMusicPercent()
{
#ifndef _WIN64
    uint8_t percent = 0;

    if (music::audio->getAudioFileDuration())
    {
        percent = music::audio->getAudioCurrentTime() / 2 * 100 / music::audio->getAudioFileDuration();
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
