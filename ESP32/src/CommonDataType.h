/**
 * @brief Common data type
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

#define STR(a) #a

#ifdef _WIN64
#include <Windows.h>
#define PROGMEM
#define MILLISEC_GET ::GetTickCount64()
#define debug_println(a) std::cout << std::string(a) << "\n"
#define debug_println_func(a) std::cout << __func__ << " -> " << std::string(a) << "\n"
#else
#include <Arduino.h>
#define MILLISEC_GET millis()
#define debug_println(a) Serial.println(std::string(a).c_str())
#define debug_println_func(a) Serial.println(String(__FUNCTION__) + " -> " + std::string(a).c_str())

#define _countof(a) sizeof(a) / sizeof(a[0])
#endif

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
#define EFFECT_WAIT_TIME 1000

#define PLAYER1_ANGLE 1800 // Left
#define PLAYER2_ANGLE 0 // Right

#define MAX_PICK_ITEM_PER_ROUND 3
#define MAX_ITEM_NUM 6
#define MAX_BULLET 8
#define MIN_BULLET 2
#define MAX_HP 4

#define MSG_OUT_OF_SPACE "Out of space!"
#define MSG_END_TURN "End turn!"
#define MSG_TOUCH_SHOTGUN "Touch the shotgun!"
#define MSG_BURNERPHONE "%d%s shell is... %s!"
#define MSG_HOW_UNFORTUNATE "How Unfortunate..."

#define DEFAULT_VOLUME 10

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
    MAX,
    // Additional, for card info only
    LIVE,
    BLANK,
    NORMAL_CHARGE,
    FADED_CHARGE,
};

enum class BULLET_TYPE
{
    MIN,
    DEF_BULLET_TYPE(BULLET_TYPE, TO_ENUM)
    MAX
};

enum class SOUND_TYPE
{
    MIN,
    DEF_SOUND_TYPE(SOUND_TYPE, TO_ENUM)
    MAX
};

enum class MUSIC_STATE_TYPE
{
    MIN,
    DEF_MUSIC_STATE_TYPE(MUSIC_STATE_TYPE, TO_ENUM)
    MAX
};

EXTERN_MAP_ENUM_STR(PLAYER_TYPE)
EXTERN_MAP_ENUM_STR(STATE_TYPE)
EXTERN_MAP_ENUM_STR(ITEM_TYPE)
EXTERN_MAP_ENUM_STR(BULLET_TYPE)
EXTERN_MAP_ENUM_STR(SOUND_TYPE)
EXTERN_MAP_ENUM_STR(MUSIC_STATE_TYPE)

struct card_info_t
{
    std::vector<std::string> usageLines;
    std::vector<std::string> strategyLines;
};

extern std::unordered_map<ITEM_TYPE, std::string> mapCardInfoName;
extern std::unordered_map<ITEM_TYPE, card_info_t> mapCardInfoDesc;

#endif // !_COMMON_DATATYPE_H
