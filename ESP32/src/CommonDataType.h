/**
 * @brief Common data types, enums, and macros
 */

#ifndef _COMMON_DATATYPE_H
#define _COMMON_DATATYPE_H

#include <cstdint>
#include <queue>
#include <tuple>
#include <vector>
#include <array>
#include <string>
#include <functional>
#include <unordered_map>
#include <algorithm>

#define STR(a) #a // Macro to convert to string

#ifdef _WIN64
#include <Windows.h>
#define PROGMEM // Null macro for PC environment compatibility
#define MILLISEC_GET ::GetTickCount64() // Get system uptime in ms for Windows
#define debug_println(a) std::cout << std::string(a) << "\n" // Console print for Windows
#define debug_println_func(a) std::cout << __func__ << " -> " << std::string(a) << "\n" // Function-prefixed print for Windows
#else
#include <Arduino.h>
#define MILLISEC_GET millis() // Get system uptime in ms for Arduino
#define debug_println(a) Serial.println(std::string(a).c_str()) // Serial print for ESP32
#define debug_println_func(a) Serial.println(String(__FUNCTION__) + " -> " + std::string(a).c_str()) // Function-prefixed print for ESP32

#define _countof(a) sizeof(a) / sizeof(a[0]) // Count number of elements in array
#endif

// Enum conversion macros
#define TO_ENUM(enum_name, item) item,
#define TO_STRING(enum_name, item) { enum_name::item, #item },

// Macro to create a map of enum values to strings
#define MAP_ENUM_STR(enum_name, enum_def) std::unordered_map<enum_name, std::string> map_##enum_name = { enum_def(enum_name,TO_STRING) };
#define EXTERN_MAP_ENUM_STR(enum_name) extern std::unordered_map<enum_name, std::string> map_##enum_name;

#ifdef _WIN64
#define TO_WSTRING(enum_name, item) { enum_name::item, L#item },
#define MAP_ENUM_WSTR(enum_name, enum_def) std::unordered_map<enum_name, std::wstring> mapWstr_##enum_name = { enum_def(enum_name,TO_WSTRING) };
#define EXTERN_MAP_ENUM_WSTR(enum_name) extern std::unordered_map<enum_name, std::wstring> mapWstr_##enum_name;
#endif

#define WAIT_TIME 3000 // General wait duration in ms
#define EFFECT_WAIT_TIME 1000 // Effect display duration in ms

#define PLAYER1_ANGLE 1800 // Angle for player 1 UI elements (180 degrees)
#define PLAYER2_ANGLE 0 // Angle for player 2 UI elements (0 degrees)

#define MAX_PICK_ITEM_PER_ROUND 3 // Max items a player can draw per round
#define MAX_ITEM_NUM 6 // Max items in player inventory
#define MAX_BULLET 8 // Max bullets in shotgun
#define MIN_BULLET 2 // Min bullets in shotgun
#define MAX_HP 4 // Max health points

// Game message strings
#define MSG_OUT_OF_SPACE "Out of space!"
#define MSG_END_TURN "End turn!"
#define MSG_TOUCH_SHOTGUN "Touch the shotgun!"
#define MSG_BURNERPHONE "%d%s shell is... %s!"
#define MSG_HOW_UNFORTUNATE "How Unfortunate..."

#define DEFAULT_VOLUME 10 // Default volume level

// Enum definition macros
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
        CREATE(e, ACTION_ITEM) \
        CREATE(e, UPDATE_HP) \
        CREATE(e, SHOTGUN_SHOT) \
        CREATE(e, RESULT) \

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

#define DEF_SOUND_TYPE(e, CREATE) \
        CREATE(e, SHOTGUN_SHOT) \
        CREATE(e, LOAD_SHELL) \
        CREATE(e, DROP) \
        CREATE(e, PICK) \
        CREATE(e, ASSIGN) \
        CREATE(e, HIT) \
        CREATE(e, WEAPON_PICKUP) \
        CREATE(e, BREAK) \
        CREATE(e, HEALTH) \
        CREATE(e, ADRENALINE) \

#define DEF_MUSIC_STATE_TYPE(e, CREATE) \
        CREATE(e, PLAY) \
        CREATE(e, PAUSE) \
        CREATE(e, RESUME) \
        CREATE(e, STOP) \
        CREATE(e, MSG_EOF) \

enum class PLAYER_TYPE // Player identifiers
{
    MIN,
    DEF_PLAYER_TYPE(PLAYER_TYPE, TO_ENUM)
    MAX
};

enum class STATE_TYPE // Game state identifiers
{
    MIN,
    DEF_STATE_TYPE(STATE_TYPE, TO_ENUM)
    MAX
};

enum class ITEM_TYPE // Item types and visual identifiers
{
    MIN,
    DEF_ITEM_TYPE(ITEM_TYPE, TO_ENUM)
    MAX,
    LIVE,
    BLANK,
    NORMAL_CHARGE,
    FADED_CHARGE,
};

enum class BULLET_TYPE // Bullet types
{
    MIN,
    DEF_BULLET_TYPE(BULLET_TYPE, TO_ENUM)
    MAX
};

enum class SOUND_TYPE // SFX identifiers
{
    MIN,
    DEF_SOUND_TYPE(SOUND_TYPE, TO_ENUM)
    MAX
};

enum class MUSIC_STATE_TYPE // Music playback status
{
    MIN,
    DEF_MUSIC_STATE_TYPE(MUSIC_STATE_TYPE, TO_ENUM)
    MAX
};

EXTERN_MAP_ENUM_STR(PLAYER_TYPE) // String map for PLAYER_TYPE
EXTERN_MAP_ENUM_STR(STATE_TYPE) // String map for STATE_TYPE
EXTERN_MAP_ENUM_STR(ITEM_TYPE) // String map for ITEM_TYPE
EXTERN_MAP_ENUM_STR(BULLET_TYPE) // String map for BULLET_TYPE
EXTERN_MAP_ENUM_STR(SOUND_TYPE) // String map for SOUND_TYPE
EXTERN_MAP_ENUM_STR(MUSIC_STATE_TYPE) // String map for MUSIC_STATE_TYPE

struct card_info_t // Structure for card descriptions
{
    std::vector<std::string> usageLines; // Description of how to use
    std::vector<std::string> strategyLines; // Description of strategy
};

extern std::unordered_map<ITEM_TYPE, std::string> mapCardInfoName; // Item names map
extern std::unordered_map<ITEM_TYPE, card_info_t> mapCardInfoDesc; // Item descriptions map

#endif // !_COMMON_DATATYPE_H
