/**
 * @brief Common data type
 */

#include "CommonDataType.h"

MAP_ENUM_STR(PLAYER_TYPE, DEF_PLAYER_TYPE)
MAP_ENUM_STR(STATE_TYPE, DEF_STATE_TYPE)
MAP_ENUM_STR(ITEM_TYPE, DEF_ITEM_TYPE)
MAP_ENUM_STR(BULLET_TYPE, DEF_BULLET_TYPE)
MAP_ENUM_STR(SOUND_TYPE, DEF_SOUND_TYPE)
MAP_ENUM_STR(MUSIC_STATE_TYPE, DEF_MUSIC_STATE_TYPE)

std::unordered_map<ITEM_TYPE, std::string> mapCardInfoName = {
    { ITEM_TYPE::BEER           , "Beer"             },
    { ITEM_TYPE::BURNERPHONE    , "Burner Phone"     },
    { ITEM_TYPE::CIGARETTE      , "Cigarette"        },
    { ITEM_TYPE::EXPIREDMEDICINE, "Expired Medicine" },
    { ITEM_TYPE::HANDCUFFS      , "Handcuffs"        },
    { ITEM_TYPE::HANDSAW        , "Hand Saw"         },
    { ITEM_TYPE::INVERTER       , "Inverter"         },
    { ITEM_TYPE::MAGNIFYINGGLASS, "Magnifying Glass" },
    { ITEM_TYPE::ADRENALINE     , "Adrenaline"       },
    { ITEM_TYPE::LIVE           , "Live Shell"       },
    { ITEM_TYPE::BLANK          , "Blank Shell"      },
    { ITEM_TYPE::NORMAL_CHARGE  , "Normal Charge"    },
    { ITEM_TYPE::FADED_CHARGE   , "Faded Charge"     },
};

// Referd to https://buckshot-roulette.fandom.com/wiki/Category:Items
std::unordered_map<ITEM_TYPE, card_info_t> mapCardInfoDesc = {
    {
        ITEM_TYPE::BEER,
        {
            {
                "Upon use, the user will rack the Shotgun, ejecting the current shell without firing it. If a Beer is used on the last round of a load, it ends that player's turn.",
            },
            {
                "- Beer is only useful if the Player has a low number of charges compared to the Other Player and a load has many live rounds, in which case reducing the number of live rounds available to the Other Player by racking the Shotgun may prove useful.",
                "    + This item may also prove useful if the Player is unsure of the current shell in the chamber.",
                "- Furthermore, if there are 2 shells left in the shotgun, using the Beer will make it function similarly to a Magnifying Glass, giving intel to the user, except for the fact that it will eject the current shell in the chamber.",
                //"- It is also a required item out of 3 to earn \"Digita, Orava& Koni\" achievement.",
            }
        }
    },
    {
        ITEM_TYPE::BURNERPHONE,
        {
            {
                "Upon use, the user will be told the location and type of a random shell loaded in the Shotgun if there are two or more shells left in the chamber.",
                "The phone will state the location of the random shell relative to the current shell (e.g. \"Second shell\" means the shell after the one currently in the chamber).",
                "If the player attempts to use the Burner Phone while there is only one shell left in the Shotgun's magazine, the phone will return \"How Unfortunate...\", wasting the item.",
            },
            {
                "- The Burner Phone is best used at the start of a load so you can plan future moves.",
                "- Burner Phones are extremely useful on smaller loads (e.g. 2 or 3 shell loads) where knowing the location and type of a shell is more valuable.",
                "- Like the Magnifying Glass, the Burner Phone only reveals information to the user.",
                //"- But If the Other Player uses the Burner Phone in the same round, they are given the same information as the player.",
                "- If you get a 2 or 3 shell load that's live and have beer, you can rack the shotgun until you get to that shell load and shoot the Other Player.",
                "- NEVER use the Burner Phone if there is one round left in the shotgun, as using it will display the text \"How Unfortunate...\", as previously stated, and waste the item. Always pay attention into how many rounds were loaded into the shotgun and how many were fired. When in doubt, use a Magnifying Glass instead, that is, if you have one.",
                //"    + In multiplayer mode, however, the Burner Phone won't work even if there are two rounds left in a chamber. If used, it displays the same text so it becomes useless as well. In that case, the Magnifying Glass is the only option to see a currently loaded round.",
            }
        }
    },
    {
        ITEM_TYPE::CIGARETTE,
        {
            {
                "Upon being used, the user will gain one charge.",
            },
            {
                "- Cigarette Packs will not restore charges if the user already has full charges.",
                "- Purposely using a Cigarette Pack when you're with full charges prevents anyone else from restoring their health via stealing it with Adrenaline.",
                //"- During the final round of Story Mode, Cigarette Packs will not restore lives if used after the user's defibrillator has been cut, rendering the Cigarette Pack useless.",
            }
        }
    },
    {
        ITEM_TYPE::EXPIREDMEDICINE,
        {
            {
                "Upon use, the user has a 50% chance of regaining two charges and a 50% chance of losing one.",
                //"Prior to update 1.2.1, Expired Medicine had a 40% chance of giving two charges and a 60% chance of taking one.",
            },
            {
                "- The usage of Expired Medicine is extremely risky as there is a 50/50 chance between restoring two charges and losing one, so only use it if you have a charge to spare.",
                "    + An exception is if you are 100% sure you're going to lose and have no cigarette packs.",
                "    + It is recommended not to use the expired medicine when you still have chance to take down the Other Player by taking advantage of other items.",
            }
        }
    },
    {
        ITEM_TYPE::HANDCUFFS,
        {
            {
                "Upon use, the Other Player will skip their next turn.",
            },
            {
                "- Handcuffs are unable to be applied twice in one turn, likely to prevent indefinite turn skipping.",
                "- Shooting yourself with a blank while the opponent is handcuffed skips an additional turn in addition to the effects of the handcuffs.",
            }
        }
    },
    {
        ITEM_TYPE::HANDSAW,
        {
            {
                "Upon being used, the user will saw part of the Shotgun's barrel and magazine off. The Shotgun will then deal double damage for the next turn if a live round is chambered. The barrel will regrow after the turn ends.",
            },
            {
                "- Only use a Hand Saw if you know what round is currently chambered.",
                "    + Using a Magnifying Glass or Burner Phone before using a Hand Saw is highly recommended.",
                "    + If the chambered round is a blank, do not use a Hand Saw unless you also have an Inverter, unless the Other Player has Adrenaline and can potentially shoot you with a live on their next turn.",
                "    + If you combine 2 handsaws with handcuff, it will be a full combo to destroy the Other Player unless there are only 2 lives round in the chamber.",
                "    + Sometimes, the handsaw should be used even without using a magnifying glass, because you could have a chance to kill the Other Player twice if you're lucky.",
            }
        }
    },
    {
        ITEM_TYPE::INVERTER,
        {
            {
                "Upon its use, the current shell in the chamber of the Shotgun will have its polarity reversed; a blank shell becomes a live shell, and a live shell becomes a blank shell.",
            },
            {
                "- If there are more blanks than lives, you should invert the current shell, as it will either remove a live shell from the shotgun, or give you an extra shot on the Other Player.",
                "- The Inverter can be used in situations where you need to get through an entire load, such as to obtain more Cigarette Packs to gain Charges, and thus can switch live shells into blank shells to pad out your turn and eventually get more items.",
                "    + Observe caution when turning a blank shell into a live one, as the amount of normal live shells will not decrease, possibly allowing a higher chance for the Other Player to get a shot on you.",
                "- If you get an item combination of an Inverter, Magnifying Glass, and a Hand Saw, you can close out some rounds almost immediately by finding a blank shell, turning it into a live shell, and then using the Hand Saw to double the damage.",
                "    + This strategy also works if you have Adrenaline, and the Other Player has some of the items that you need for this strategy.",
            }
        }
    },
    {
        ITEM_TYPE::MAGNIFYINGGLASS,
        {
            {
                "Upon being used, the user will see what type of round is currently loaded in the Shotgun's chamber.",
            },
            {
                "- Magnifying Glasses are very powerful when used in conjunction with Handcuffs, Hand Saw or an Inverter, and possibly the Burner Phone.",
                "- Always be prepared should the Other Player use the Magnifying Glass. After they use one, their behavior will tell you whether the round is blank or live before they fire it. If the round is blank, they will either point it at themself or use an Inverter if they can. If the round is live (naturally or through the use of an Inverter), they will point it at the Other Player, and will use a Hand saw first if possible.",
                "- If the Player uses the Magnifying Glass but doesn't have either Handsaw, Handcuffs and/or Inverter, but does have Adrenaline, should there be a Handsaw, Handcuffs or Inverter on the Other Player's side it is always a good idea to use the Adrenaline and snatch that item from the Other Player. Beware, as the Other Player may do the same thing to you.",
            }
        }
    },
    {
        ITEM_TYPE::ADRENALINE,
        {
            {
                "Upon use, the user will be able to steal one item from the Other Player's board and use it immediately. Any item can be stolen except for another Adrenaline, or Handcuffs if the opponent being stolen from is already cuffed.",
            },
            {
                //"- You only have 10 seconds to select the item you want to steal after using the Adrenaline, so decide what you want to steal before using it.",
                "- If the Other Player has at least one Adrenaline, it is generally recommended to use up items that may pose a threat if the Other Player decides to steal them.",
                "    + These include Hand Saws, Cigarette Packs, Handcuffs, Magnifying Glasses, and Inverters.",
            }
        }
    },
    {
        ITEM_TYPE::LIVE,
        {
            {
                "A live shell has a red casing and will deal one damage to whoever it is fired at, or two if the barrel is sawed off. Shooting a live round ends the user's turn regardless of who it is fired at.",
            },
            {
                "- Using a Hand Saw deals double damage for the next turn if the chambered shell is live.",
                "- Using a Inverter will invert the type of the current shell in the chamber.",
            }
        }
    },
    {
        ITEM_TYPE::BLANK,
        {
            {
                "A blank shell has a blue casing and will not deal any damage. If the user chooses to shoot themselves and the round is a blank, they gain another turn. However, if they choose to shoot the opposing player and the round is a blank, their turn ends.",
            },
            {
                "- Using a Hand Saw deals double damage for the next turn if the chambered shell is live.",
                "- Using a Inverter will invert the type of the current shell in the chamber.",
            }
        }
    },
    {
        ITEM_TYPE::NORMAL_CHARGE,
        {
            {
                "Charge can be increase with items such as: Cigarette Packs, Expired Medicine (50% chance to get increased charge)",
            },
            {
                "- When the charge number is 0, the Player cannot increase it again with rechargeable items.",
            }
        }
    },
    {
        ITEM_TYPE::FADED_CHARGE,
        {
            {
                "Charge can not be increase with items such as: Cigarette Packs, Expired Medicine. Charge can not be reduced by using Expired Medicine.",
            },
            {
                "- When the charge number is 0, the Player will lose immediately.",
            }
        }
    },
};
