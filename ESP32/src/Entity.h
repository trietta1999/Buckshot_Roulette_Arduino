/**
 * @brief Entity
 */

#ifndef _ENTITY_H
#define _ENTITY_H

#include <lvgl.h>
#include "CommonDataType.h"
#include "CommonLibrary.h"
#include "CommonService.h"

namespace player
{
    struct player_info_t
    {
        struct player_slot_button_t
        {
            // Type
            PLAYER_TYPE playerType;
            ITEM_TYPE itemType;

            // UI
            lv_obj_t* button;
            lv_obj_t* buttonImg;
            lv_img_dsc_t image;

            void Assign(PLAYER_TYPE playerType, ITEM_TYPE itemType, const void* image)
            {
                lv_image_set_src(this->buttonImg, image);
                memcpy(&this->image, image, sizeof(lv_img_dsc_t));
                this->itemType = itemType;
                this->playerType = playerType;

                PlaySoundWrapper(SOUND_TYPE::ASSIGN);
            }

            void Unassign()
            {
                memset(&this->image, 0, sizeof(this->image));
                lv_image_set_src(this->buttonImg, &ui_img_empty_png);
                this->itemType = ITEM_TYPE::MIN;
                this->playerType = PLAYER_TYPE::MIN;
            }
        };

        // Type
        PLAYER_TYPE type;

        // UI
        lv_obj_t* table;
        lv_obj_t* pickButton;
        lv_obj_t* confirmButton;
        lv_obj_t* gunfireEffect;
        lv_obj_t* adrenalinefEffect;
        std::vector<lv_obj_t*> listHPLevel1;
        std::vector<lv_obj_t*> listHPLevel2;

        // Property
        std::vector<player_slot_button_t> listButtonInfo;
        uint8_t pickItemCount;
        uint8_t totalItemCount;
        uint8_t hpLevel2;
        uint8_t hpLevel1;
        uint16_t angle;
        bool isPickComplete;
        bool isSkip;
        bool isNext;

        bool operator==(const player_info_t& other)
        {
            return (other.type == this->type);
        }

        void Enable()
        {
            lv_obj_add_state(this->table, LV_STATE_CHECKED);

            for (const auto& item : this->listButtonInfo)
            {
                lv_obj_remove_state(item.button, LV_STATE_DISABLED);
            }
        }

        void Disable()
        {
            lv_obj_add_state(this->pickButton, LV_STATE_DISABLED);
            lv_obj_remove_state(this->table, LV_STATE_CHECKED);

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
    };

    std::vector<player_info_t> listPlayer = { };

    bool CheckAllPickComplete()
    {
        bool result = true;

        // Check complete flag
        for (const auto& player : listPlayer)
        {
            if (!player.isPickComplete)
            {
                result = false;
                break;
            }
        }

        if (result)
        {
            // Reset complete flag
            for (auto& player : listPlayer)
            {
                player.isPickComplete = false;
            }
        }

        return result;
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
                player.Enable();
            }
            else
            {
                player.Disable();
            }
        }
    }

    void DisableAllPlayerTableExcept(const player_info_t& current)
    {
        for (auto& player : listPlayer)
        {
            if (player.type != current.type)
            {
                player.Disable();
            }
            else
            {
                player.Enable();
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
                    if ((lv_obj_get_state(player.listHPLevel1[i]) & LV_STATE_DISABLED) == LV_STATE_DISABLED)
                    {
                        lv_obj_remove_state(player.listHPLevel1[i], LV_STATE_DISABLED);

                        PlaySoundWrapper(SOUND_TYPE::HEALTH);
                    }
                }
                else
                {
                    if ((lv_obj_get_state(player.listHPLevel1[i]) & LV_STATE_DISABLED) != LV_STATE_DISABLED)
                    {
                        lv_obj_add_state(player.listHPLevel1[i], LV_STATE_DISABLED);

                        PlaySoundWrapper(SOUND_TYPE::HIT);
                    }
                }
            }

            // Update HP level 2
            for (uint8_t i = 0; i < player.listHPLevel2.size(); i++)
            {
                if (i < player.hpLevel2)
                {
                    if ((lv_obj_get_state(player.listHPLevel2[i]) & LV_STATE_DISABLED) == LV_STATE_DISABLED)
                    {
                        lv_obj_remove_state(player.listHPLevel2[i], LV_STATE_DISABLED);

                        PlaySoundWrapper(SOUND_TYPE::HEALTH);
                    }
                }
                else
                {
                    if ((lv_obj_get_state(player.listHPLevel2[i]) & LV_STATE_DISABLED) != LV_STATE_DISABLED)
                    {
                        lv_obj_add_state(player.listHPLevel2[i], LV_STATE_DISABLED);

                        PlaySoundWrapper(SOUND_TYPE::HIT);
                    }
                }
            }
        }
    }

    bool CheckAnyPlayerDead()
    {
        for (auto& player : listPlayer)
        {
            if (!player.hpLevel1)
            {
                return true;
            }
        }

        return false;
    }
}

namespace shotgun
{
    struct shotgun_info_t
    {
        // UI
        lv_obj_t* objInTable;
        lv_obj_t* objInside;
        lv_obj_t* objHand;
        lv_obj_t* objWndConfirm;
        lv_obj_t* objTrashBullet;
        std::vector<lv_obj_t*> listBulletImg;

        // Property
        std::unordered_map<BULLET_TYPE, lv_img_dsc_t> mapBulletImg;
        std::vector<BULLET_TYPE> listBullet;
        std::queue<BULLET_TYPE> queueBullet;
        bool isCut;
        bool isGunfire;
        uint8_t state;
        uint8_t currentBulletIndex;
        player::player_info_t* targetPlayer;
        player::player_info_t* currentPlayer;

        void Disable()
        {
            lv_obj_add_state(this->objInTable, LV_STATE_DISABLED);
        }

        void Enable()
        {
            lv_obj_remove_state(this->objInTable, LV_STATE_DISABLED);
        }

        std::vector<BULLET_TYPE> QueueToVec()
        {
            std::vector<BULLET_TYPE> vec = { };
            std::queue<BULLET_TYPE> tempQueue = this->queueBullet;

            while (!tempQueue.empty())
            {
                vec.push_back(tempQueue.front());
                tempQueue.pop();
            }

            return vec;
        }

        void VecToQueue(std::vector<BULLET_TYPE> vec)
        {
            this->queueBullet = std::queue<BULLET_TYPE>(); // Empty queue

            for (const auto& item : vec)
            {
                this->queueBullet.push(item);
            }
        }

        void Reset()
        {
            // Update shotgun image
            lv_image_set_src(this->objInTable, &ui_img_shotgun_png);
            lv_image_set_src(this->objHand, &ui_img_shotgun_png);

            this->isCut = false;
            this->isGunfire = false;
        }

        void Cut()
        {
            // Update shotgun image
            lv_image_set_src(this->objInTable, &ui_img_shotguncut_png);
            lv_image_set_src(this->objHand, &ui_img_shotguncut_png);

            this->isCut = true;

            PlaySoundWrapper(SOUND_TYPE::BREAK);
        }

        void ShowInHand(player::player_info_t& player)
        {
            lv_obj_remove_flag(this->objWndConfirm, LV_OBJ_FLAG_HIDDEN);

            // Rotate shotgun to current player
            lv_image_set_rotation(this->objHand, player.angle);


            // Uncheck all confirm button
            for (const auto& iPlayer : player::listPlayer)
            {
                lv_obj_remove_state(iPlayer.confirmButton, LV_STATE_CHECKED);
            }

            // Check current player confirm button
            lv_obj_add_state(player.confirmButton, LV_STATE_CHECKED);

            PlaySoundWrapper(SOUND_TYPE::WEAPON_PICKUP);
        }

        void Shot()
        {
            // Rotate shotgun to target player
            if (this->state == 0)
            {
                lv_image_set_rotation(this->objHand, this->targetPlayer->angle);
            }
            // Show shot effect
            else if (this->state == 1)
            {
                this->currentBulletIndex++;

                lv_image_set_src(this->objTrashBullet, &this->mapBulletImg[this->queueBullet.front()]);

                if (this->queueBullet.front() == BULLET_TYPE::BLANK)
                {
                    this->queueBullet.pop(); // Remove bullet

                    PlaySoundWrapper(SOUND_TYPE::DROP);
                    return;
                }
                else
                {
                    lv_obj_remove_flag(this->targetPlayer->gunfireEffect, LV_OBJ_FLAG_HIDDEN); // Show effect

                    PlaySoundWrapper(SOUND_TYPE::SHOTGUN_SHOT);

                    this->queueBullet.pop(); // Remove bullet
                    this->isGunfire = true;

                    if (this->isCut)
                    {
                        if (targetPlayer->hpLevel2 >= 2)
                        {
                            targetPlayer->hpLevel2 -= 2;
                        }
                        else if (targetPlayer->hpLevel2 == 1)
                        {
                            targetPlayer->hpLevel2--;
                            targetPlayer->hpLevel1--;
                        }
                        else
                        {
                            if (targetPlayer->hpLevel1 >= 2)
                            {
                                targetPlayer->hpLevel1 -= 2;
                            }
                            else if (targetPlayer->hpLevel1 == 1)
                            {
                                targetPlayer->hpLevel1--;
                            }
                        }
                    }
                    else
                    {
                        if (targetPlayer->hpLevel2 > 0)
                        {
                            targetPlayer->hpLevel2--;
                        }
                        else if (targetPlayer->hpLevel1 > 0)
                        {
                            targetPlayer->hpLevel1--;
                        }
                    }
                }

                // Wait for hide effect
                DelayCallback(MAKE_DELAY_CB{
                    decltype(this) ldata = (decltype(this))(data);
                    lv_obj_add_flag(ldata->targetPlayer->gunfireEffect, LV_OBJ_FLAG_HIDDEN);
                    }, this, EFFECT_WAIT_TIME);
            }
            // Reset shotgun in hand
            else if (this->state == 2)
            {
                lv_obj_add_flag(this->targetPlayer->gunfireEffect, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(this->objWndConfirm, LV_OBJ_FLAG_HIDDEN);
            }
        }
    };
}

#endif // !_ENTITY_H
