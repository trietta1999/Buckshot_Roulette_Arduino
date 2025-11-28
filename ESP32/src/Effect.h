#pragma once

#define EFFECT_FUNCTION(item) static bool Play_##item##_Effect(player::player_info_t::player_slot_button_t& buttonSlot)
#define EFFECT_FUNCTION_CALLBACK(item) Play_##item##_Effect

EFFECT_FUNCTION(BEER)
{
    if (!Shotgun.queueBullet.empty())
    {
        buttonSlot.Unassign();

        ShowEjectedBullet();
        lv_image_set_src(ui_imgEjectedBullet, &Shotgun.mapBulletImg[Shotgun.queueBullet.front()]);

        Shotgun.queueBullet.pop(); // Remove first bullet
    }

    // Out of bullet
    if (!Shotgun.queueBullet.size())
    {
        // Wait for transit to next round
        DelayCallback(MAKE_DELAY_CB{
            FSMTransit(STATE_TYPE::PLAYER_NEXT);
            }, nullptr, EFFECT_WAIT_TIME);
    }

    itemUsingState = false;

    return true;
}

EFFECT_FUNCTION(BURNERPHONE)
{
    if (!Shotgun.queueBullet.empty())
    {
        buttonSlot.Unassign();

        // Show message
        lv_obj_remove_flag(ui_lblCardMessage, LV_OBJ_FLAG_HIDDEN);

        if (Shotgun.queueBullet.size() == 1)
        {
            lv_label_set_text(ui_lblCardMessage, MSG_HOW_UNFORTUNATE);
        }
        else
        {
            auto bulletOrder = RandomRangeNumber(0, Shotgun.queueBullet.size() - 1);
            auto bulletType = map_BULLET_TYPE[Shotgun.listBullet[bulletOrder]];
            std::string suffix = "";

            GetOrdinalNumber(++bulletOrder, suffix);

            lv_label_set_text_fmt(ui_lblCardMessage, MSG_BURNERPHONE, bulletOrder, suffix.c_str(), bulletType.c_str());
        }

        // Show delayed message
        DelayCallback(MAKE_DELAY_CB{
            // Hide message
            lv_obj_add_flag(ui_lblCardMessage, LV_OBJ_FLAG_HIDDEN);

            itemUsingState = false; // Unblock
            }, nullptr, WAIT_TIME);
    }

    return true;
}

EFFECT_FUNCTION(CIGARETTE)
{
    buttonSlot.Unassign();

    // For HP level 2 only
    if ((Player->hpLevel2 > 0) && (Player->hpLevel2 < MAX_HP))
    {
        Player->hpLevel2++;
    }

    FSMTransit(STATE_TYPE::UPDATE_HP);

    itemUsingState = false; // Unblock

    return true;
}

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

EFFECT_FUNCTION(HANDCUFFS)
{
    bool result = false;
    auto& nextPlayer = player::NextPlayer(*Player);

    if (!nextPlayer.isSkip)
    {
        buttonSlot.Unassign();

        nextPlayer.isSkip = true;

        result = true;
    }

    itemUsingState = false;

    return result;
}

EFFECT_FUNCTION(HANDSAW)
{
    bool result = false;

    if (!Shotgun.isCut)
    {
        buttonSlot.Unassign();

        Shotgun.Cut();

        result = true;
    }

    itemUsingState = false;

    return result;
}

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

        Shotgun.VecToQueue(listBullet);
    }

    itemUsingState = false;

    return true;
}

EFFECT_FUNCTION(MAGNIFYINGGLASS)
{
    buttonSlot.Unassign();

    // Show shotgun inside
    lv_obj_remove_flag(Shotgun.objInside, LV_OBJ_FLAG_HIDDEN);

    // Rotate shotgun inside to current player
    lv_obj_set_style_transform_rotation(Shotgun.objInside, Player->angle, LV_PART_MAIN | LV_STATE_DEFAULT);

    return true;
}

EFFECT_FUNCTION(ADRENALINE)
{
    player::EnableAllPlayerTableExcept(*Player);

    itemUsingState = false;

    lv_obj_remove_flag(ui_wndAdrenalineEffect, LV_OBJ_FLAG_HIDDEN); // Show effect
    lv_obj_set_style_transform_rotation(ui_wndAdrenalineEffect, Player->angle, LV_PART_MAIN | LV_STATE_DEFAULT); // Rotate to current player

    return true;
}
