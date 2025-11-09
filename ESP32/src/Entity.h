/**
 * @brief Entity
 */

#ifndef _ENTITY_H
#define _ENTITY_H

#include <lvgl.h>
#include "CommonDataType.h"

namespace player
{
    struct player_info_t
    {
        struct player_slot_button_t
        {
            ITEM_TYPE itemType;
            lv_obj_t* button;
            lv_obj_t* buttonImg;
            lv_img_dsc_t image;
        };

        PLAYER_TYPE type;
        lv_obj_t* pickButton;
        std::vector<player_slot_button_t> listButtonInfo;
        std::vector<lv_obj_t*> listHPLevel1;
        std::vector<lv_obj_t*> listHPLevel2;
        uint8_t pickItemCount;
        uint8_t totalItemCount;
        uint8_t hpLevel2;
        uint8_t hpLevel1;
        bool isPickComplete;

        void Disable()
        {
            lv_obj_add_state(this->pickButton, LV_STATE_DISABLED);

            for (const auto& item : this->listButtonInfo)
            {
                lv_obj_add_state(item.button, LV_STATE_DISABLED);
            }
        }

        void EnablePickButton()
        {
            lv_obj_remove_state(this->pickButton, LV_STATE_DISABLED);
        }

        void DisablePickButton()
        {
            lv_obj_add_state(this->pickButton, LV_STATE_DISABLED);
        }

        void EnableTable()
        {
            for (const auto& item : this->listButtonInfo)
            {
                lv_obj_remove_state(item.button, LV_STATE_DISABLED);
            }
        }
    };

    std::vector<player_info_t> listPlayer = { };

    bool AllPickComplete()
    {
        for (const auto& player : listPlayer)
        {
            if (!player.isPickComplete)
            {
                return false;
            }
        }

        return true;
    }

    player_info_t& NextPlayer(const player_info_t& current)
    {
        for (auto& player : listPlayer)
        {
            if (player.type > current.type)
            {
                return player;
            }
        }

        return listPlayer[0]; // First player
    }

    void EnableAllPlayerTableExcept(const player_info_t& current)
    {
        for (auto& player : listPlayer)
        {
            if (player.type != current.type)
            {
                player.EnableTable();
            }
        }
    }

    void UpdateAllPlayerHP()
    {
        for (auto& player : listPlayer)
        {
            // Update HP level 1
            for (uint8_t i = 0; i < player.listHPLevel1.size(); i++)
            {
                if (i < player.hpLevel1)
                {
                    lv_obj_remove_state(player.listHPLevel1[i], LV_STATE_DISABLED);
                }
                else
                {
                    lv_obj_add_state(player.listHPLevel1[i], LV_STATE_DISABLED);
                }
            }

            // Update HP level 2
            for (uint8_t i = 0; i < player.listHPLevel2.size(); i++)
            {
                if (i < player.hpLevel2)
                {
                    lv_obj_remove_state(player.listHPLevel2[i], LV_STATE_DISABLED);
                }
                else
                {
                    lv_obj_add_state(player.listHPLevel2[i], LV_STATE_DISABLED);
                }
            }
        }
    }
};

namespace shotgun
{
    struct shotgun_info_t
    {
        lv_obj_t* objInTable;
        lv_obj_t* objInside;
        lv_obj_t* objHand;
        std::unordered_map<BULLET_TYPE, lv_img_dsc_t> mapBulletImg;
        std::vector<lv_obj_t*> listBulletImg;
        std::vector<BULLET_TYPE> listBullet;
        std::queue<BULLET_TYPE> queueBullet;
        BULLET_TYPE invertedBullet;
        bool isCut;

        void Disable()
        {
            lv_obj_add_state(this->objInTable, LV_STATE_DISABLED);
        }

        void Enable()
        {
            lv_obj_remove_state(this->objInTable, LV_STATE_DISABLED);
        }

        void VecToQueue()
        {
            this->queueBullet = std::queue<BULLET_TYPE>(); // Empty queue

            for (const auto& item : this->listBullet)
            {
                this->queueBullet.push(item);
            }
        }
    };
};

#endif // !_ENTITY_H
