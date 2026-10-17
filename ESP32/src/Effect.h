#pragma once

/**
 * @brief Logic for game item effects
 */

// Macros to define and reference effect handlers for each item type
#define EFFECT_FUNCTION(item) static bool Play_##item##_Effect(player::player_info_t::player_slot_button_t& buttonSlot)
#define EFFECT_FUNCTION_CALLBACK(item) Play_##item##_Effect

// BEER: Ejects the current shell from the shotgun without firing
EFFECT_FUNCTION(BEER)
{
    if (!Shotgun.queueBullet.empty())
    {
        buttonSlot.Unassign(); // Remove item from inventory

        ShowEjectedBullet(); // Show animation of bullet flying out
        lv_image_set_src(ui_imgEjectedBullet, &Shotgun.mapBulletImg[Shotgun.queueBullet.front()]);

        Shotgun.queueBullet.pop(); // Remove the ejected shell from magazine
    }

    // Check if the magazine is now empty
    if (!Shotgun.queueBullet.size())
    {
        // Wait for transit to next round
        DelayCallback(MAKE_DELAY_CB{
            FSMTransit(STATE_TYPE::PLAYER_NEXT);
            }, nullptr, EFFECT_WAIT_TIME);
    }

    itemUsingState = false; // Release interaction lock

    return true;
}

// BURNERPHONE: Reveals a random future shell in the magazine
EFFECT_FUNCTION(BURNERPHONE)
{
    if (!Shotgun.queueBullet.empty())
    {
        buttonSlot.Unassign();

        if (Shotgun.queueBullet.size() == 1) // Only one shell left, can't reveal "future"
        {
            lv_label_set_text(ui_lblMessageInside, MSG_HOW_UNFORTUNATE);
        }
        else
        {
            // Pick a random index between current and end of magazine
            int32_t bulletOrder = RandomRangeNumber(Shotgun.currentBulletIndex, Shotgun.listBullet.size() - 1);
            auto bulletType = map_BULLET_TYPE[Shotgun.listBullet[bulletOrder]];
            std::string suffix = "";

            GetOrdinalNumber(++bulletOrder, suffix);

            lv_label_set_text_fmt(ui_lblMessageInside, MSG_BURNERPHONE, bulletOrder, suffix.c_str(), bulletType.c_str());
        }

        lv_obj_remove_flag(Shotgun.objInside, LV_OBJ_FLAG_HIDDEN); // Show shotgun detail view
    }

    return true;
}

// CIGARETTE: Restores 1 HP
EFFECT_FUNCTION(CIGARETTE)
{
    buttonSlot.Unassign();

    // For HP level 2 only
    if ((Player->hpLevel2 > 0) && (Player->hpLevel2 < MAX_HP))
    {
        Player->hpLevel2++;
    }

    FSMTransit(STATE_TYPE::UPDATE_HP);

    itemUsingState = false;

    return true;
}

// EXPIREDMEDICINE: 50/50 chance to restore 2 HP or lose 1 HP
EFFECT_FUNCTION(EXPIREDMEDICINE)
{
    buttonSlot.Unassign();

    auto num = RandomRangeNumber(0, 100);

    // HP level 2 can increase, but HP level 1 can not
    if (num % 2 == 0) // Increase HP
    {
        if (Player->hpLevel2 > 0)
        {
            if (Player->hpLevel2 < MAX_HP - 1) // HP: 1 ~ 2
            {
                Player->hpLevel2 += 2;
            }
            else if (Player->hpLevel2 == MAX_HP - 1) // HP: 3
            {
                Player->hpLevel2++;
            }
        }
    }
    else if (num % 2 != 0) // Decrease HP
    {
        // Only HP level 2
        if (Player->hpLevel2 > 0)
        {
            Player->hpLevel2--;
        }
    }

    FSMTransit(STATE_TYPE::UPDATE_HP);

    itemUsingState = false; // Unblock

    return true;
}

// HANDCUFFS: Skips the opponent's next turn
EFFECT_FUNCTION(HANDCUFFS)
{
    bool result = false;
    auto& nextPlayer = player::NextPlayer(*Player);

    if (!nextPlayer.isSkip) // Can't stack handcuffs
    {
        buttonSlot.Unassign();

        nextPlayer.SetSkip(true);

        result = true;

        PlaySoundWrapper(SOUND_TYPE::LOAD_SHELL);
    }

    itemUsingState = false;

    return result;
}

// HANDSAW: Doubles the damage of the next 'Live' shell
EFFECT_FUNCTION(HANDSAW)
{
    bool result = false;

    if (!Shotgun.isCut) // Only one saw per shot
    {
        buttonSlot.Unassign();

        Shotgun.Cut();

        result = true;
    }

    itemUsingState = false;

    return result;
}

// INVERTER: Swaps the type of the current shell (Live <-> Blank)
EFFECT_FUNCTION(INVERTER)
{
    if (!Shotgun.queueBullet.empty())
    {
        buttonSlot.Unassign();

        auto listBullet = Shotgun.QueueToVec();

        if (listBullet[0] == BULLET_TYPE::LIVE)
        {
            listBullet[0] = BULLET_TYPE::BLANK;
        }
        else if (listBullet[0] == BULLET_TYPE::BLANK)
        {
            listBullet[0] = BULLET_TYPE::LIVE;
        }

        Shotgun.VecToQueue(listBullet); // Re-sync magazine

        PlaySoundWrapper(SOUND_TYPE::LOAD_SHELL);
    }

    itemUsingState = false;

    return true;
}

// MAGNIFYINGGLASS: Reveals the type of the current shell
EFFECT_FUNCTION(MAGNIFYINGGLASS)
{
    buttonSlot.Unassign();

    lv_obj_remove_flag(Shotgun.objInside, LV_OBJ_FLAG_HIDDEN); // Show shotgun inside

    return true;
}

// ADRENALINE: Allows player to steal and use an item from the opponent
EFFECT_FUNCTION(ADRENALINE)
{
    buttonSlot.Unassign();

    player::EnableAllPlayerTableExcept(*Player); // Focus on opponent's table

    itemUsingState = false;

    // Trigger visual effects
    lv_obj_remove_flag(ui_wndAdrenalineEffect, LV_OBJ_FLAG_HIDDEN);
    lv_obj_remove_flag(Player->adrenalinefEffect, LV_OBJ_FLAG_HIDDEN);

    Shotgun.Disable(); // Disable shotgun while stealing

    PlaySoundWrapper(SOUND_TYPE::ADRENALINE);

    return true;
}
