/**
 * @brief Common data type
 */

#ifndef _COMMON_DATATYPE_H
#define _COMMON_DATATYPE_H

#include <cstdint>
#include <tuple>
#include <vector>
#include <array>
#include <string>
#include <unordered_map>

#ifndef HOST_TIMER
#define HOST_TIMER
#endif

#define STR(a) #a

#ifdef _WIN64
#define HOST_NAME L"Buckshot_Roulette"
#define CLIENT_NAME L"Buckshot_Roulette"
#define CLIENT_NAME_FOR_JSON "Buckshot_Roulette"
#else
#define HOST_NAME map_MODULE_NAME[MODULE_NAME::HostTimer].c_str()
#define CLIENT_NAME map_MODULE_NAME[MODULE_NAME::HostTimer].c_str()
#endif

#define MAX_SIZE 1000
#define BUFFER_SIZE sizeof(char) * MAX_SIZE

#ifdef _WIN64
#include <Windows.h>
#define MILLISEC_GET ::GetTickCount64()
#define debug_println(a) std::cout << std::string(a) << "\n"
#define SHARED_MEM L"SharedMemoryJson"
#else
#include <Arduino.h>
#define WM_USER 0x0400
#define HWND uint8_t

#define debug_println(a) Serial.println(std::string(a).c_str())
#define MILLISEC_GET millis()

#define INPUT_PIN 22
#define BUZZER_PIN 26

#define IP_ADD_1 192
#define IP_ADD_2 168
#define IP_ADD_3 0
#define IP_ADD_4 1

#define NETMASK_1 255
#define NETMASK_2 255
#define NETMASK_3 255
#define NETMASK_4 0

#define HTTP_OK 200
#define HTTP_NOT_FOUND 404

struct data_pack_t {
    char source[20];
    uint32_t base_msg;
    uint32_t msg;
    char data[MAX_SIZE];
};
#endif

enum
{
    WM_USER_MIN = WM_USER,
    WM_SET_CLIENT_HANDLE, // Save client handle
    WM_REQUEST,           // Request/get from other message
    WM_REQUEST_WITH_DATA, // Request/get from other message with data
    WM_RESPONSE,          // Response message from host to client
    WM_CLIENT_RESPONSE,   // Response message from client to host
    WM_SET_CLIENTSTATE,   // Notify to set ON/OFF status to selected clients
    WM_TIMER_GET,         // Client get timer from host
    WM_STRIKENUM_GET,     // Client get strike count from host
    WM_STRIKESTATE_SET,   // Client notify host to set strike status
    WM_SUCCESSSTATE_SET,  // Client notify host to set client success status
    WM_SYSINIT_GET,       // Client get init system data from host
    WM_STOP_ALL,          // Notify for stoping all client
    WM_STOP_COMPLETE,     // Notify for all client stopped to Host
};

#define INCORRECT (uint8_t)(-1)

#define BEEP_FRE 3000
#define BEEP_INCREASE_DURATION 50
#define BEEP_TIMEOUT 3000

#define STATE_CHECKED 3
#define STATE_UNCHECK 2

#define TO_ENUM(enum_name, item) item,
#define TO_STRING(enum_name, item) { enum_name::item, #item },

#define MAP_ENUM_STR(enum_name, enum_def) std::unordered_map<enum_name, std::string> map_##enum_name = { enum_def(enum_name,TO_STRING) };
#define EXTERN_MAP_ENUM_STR(enum_name) extern std::unordered_map<enum_name, std::string> map_##enum_name;

#ifdef _WIN64
#define TO_WSTRING(enum_name, item) { enum_name::item, L#item },
#define MAP_ENUM_WSTR(enum_name, enum_def) std::unordered_map<enum_name, std::wstring> mapWstr_##enum_name = { enum_def(enum_name,TO_WSTRING) };
#define EXTERN_MAP_ENUM_WSTR(enum_name) extern std::unordered_map<enum_name, std::wstring> mapWstr_##enum_name;
#endif

#define WAIT_TIME 3000

#define PLAYER1_ANGLE 0
#define PLAYER2_ANGLE 1800
#define STEP_ANGLE 100
#define MAX_PICK_ITEM_PER_ROUND 4
#define MAX_ITEM_NUM 8
#define MAX_BULLET 8
#define MIN_BULLET 2
#define MAX_HP 4

#define MSG_OUT_OF_SPACE "OUT OF SPACE!"
#define MSG_END_TURN "END TURN!"
#define MSG_TOUCH_SHOTGUN "Touch the shotgun!"

#define DEF_PLAYER_TYPE(e, CREATE) \
        CREATE(e, PLAYER1) \
        CREATE(e, PLAYER2) \

#define DEF_STATE_TYPE(e, CREATE) \
        CREATE(e, STARTUP) \
        CREATE(e, PICK) \
        CREATE(e, PLAYER_NEXT) \
        CREATE(e, PLAYER_ITEM_ARRANGE) \
        CREATE(e, LOAD_SHELL) \
        CREATE(e, ACTION_TURN) \

#define DEF_ITEM_TYPE(e, CREATE) \
        CREATE(e, BEER) \
        CREATE(e, BURNERPHONE) \
        CREATE(e, CIGARETTE) \
        CREATE(e, EXPIREDMEDICINE) \
        CREATE(e, HANDCUFFS) \
        CREATE(e, HANDSAW) \
        CREATE(e, INVERTER) \
        CREATE(e, MAGNIFYINGGLASS) \
        CREATE(e, ADRENALINE) \

#define DEF_BULLET_TYPE(e, CREATE) \
        CREATE(e, BLANK) \
        CREATE(e, LIVE) \

enum class PLAYER_TYPE
{
    MIN,
    DEF_PLAYER_TYPE(PLAYER_TYPE, TO_ENUM)
    MAX
};

enum class STATE_TYPE
{
    MIN,
    DEF_STATE_TYPE(STATE_TYPE, TO_ENUM)
    MAX
};

enum class ITEM_TYPE
{
    MIN,
    DEF_ITEM_TYPE(ITEM_TYPE, TO_ENUM)
    MAX
};

enum class BULLET_TYPE
{
    MIN,
    DEF_BULLET_TYPE(BULLET_TYPE, TO_ENUM)
    MAX
};

EXTERN_MAP_ENUM_STR(PLAYER_TYPE)
EXTERN_MAP_ENUM_STR(STATE_TYPE)
EXTERN_MAP_ENUM_STR(ITEM_TYPE)
EXTERN_MAP_ENUM_STR(BULLET_TYPE)

#endif // !_COMMON_DATATYPE_H
