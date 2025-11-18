#pragma once

void FSMTransit(STATE_TYPE next)
{
    CurrentState.SetValue(next);

    if (CurrentState.GetState())
    {
        FSM();
    }
}

void FSM()
{
#pragma region Common_state
    // Startup state
    if (CurrentState.GetValue() == STATE_TYPE::STARTUP)
    {
        if (CheckObjectState(ui_btnPlayer1Start, LV_STATE_CHECKED) && CheckObjectState(ui_btnPlayer2Start, LV_STATE_CHECKED))
        {
            // Change screen to main screen
            _ui_screen_change(&ui_Main, LV_SCR_LOAD_ANIM_FADE_IN, 500, 0, &ui_Main_screen_init);

            FSMTransit(STATE_TYPE::PLAYER_NEXT);
        }
    }

    // Shotgun shot
    if (CurrentState.GetValue() == STATE_TYPE::SHOTGUN_SHOT)
    {
        if (MILLISEC_GET - lastShotgunTime >= EFFECT_WAIT_TIME)
        {
            // End stage of shotgun
            if (Shotgun.state > 2)
            {
                FSMTransit(STATE_TYPE::UPDATE_HP);

                // Shotgun is fired or Target player is not current player
                if ((Shotgun.isGunfire) || (Player != Shotgun.targetPlayer))
                {
                    FSMTransit(STATE_TYPE::ACTION_TURN);
                }

                ShowTrashBullet();

                Shotgun.Reset();

                // Out of bullet
                if (!Shotgun.queueBullet.size())
                {
                    // Wait for transit to next round
                    lv_timer_create([](lv_timer_t* timer) {
                        FSMTransit(STATE_TYPE::PLAYER_NEXT);
                        }, EFFECT_WAIT_TIME, nullptr)->repeat_count = 1;
                }
            }

            // Execute shotgun
            Shotgun.Shot();
            Shotgun.state++;

            lastShotgunTime = MILLISEC_GET;
        }
    }
#pragma endregion

    if (CurrentState.GetState())
    {
        debug_println("State: " + map_STATE_TYPE[CurrentState.GetValue()]);

        // Update player sequence
        if (CurrentState.GetValue() == STATE_TYPE::PLAYER_NEXT)
        {
            ResetPlayerTable();

            if (player::CheckAllPickComplete())
            {
                Shotgun.RotateToPlayer(Player->angle);

                FSMTransit(STATE_TYPE::LOAD_SHELL);
            }
            // Move to next player
            else
            {
                if (!Player)
                {
                    // Set player #1 as default
                    Player = &player::listPlayer[0];
                }
                else
                {
                    Player = &player::NextPlayer(*Player);
                }

                Shotgun.RotateToPlayer(Player->angle);

                FSMTransit(STATE_TYPE::PICK);
            }
        }
        // Item pick state
        else if (CurrentState.GetValue() == STATE_TYPE::PICK)
        {
            Player->EnablePickButton();
        }
        // Item arrange state
        else if (CurrentState.GetValue() == STATE_TYPE::PLAYER_ITEM_ARRANGE)
        {
            Player->EnableTable();
        }
        // Load shell state
        else if (CurrentState.GetValue() == STATE_TYPE::LOAD_SHELL)
        {
            uint8_t bulletNum = RandomRangeNumber(MIN_BULLET, MAX_BULLET);

            Shotgun.listBullet = CreateBulletList(bulletNum); // Random order
            Shotgun.VecToQueue(Shotgun.listBullet);

            std::vector<BULLET_TYPE> listBulletSort = Shotgun.listBullet;
            std::sort(listBulletSort.begin(), listBulletSort.end(), [](BULLET_TYPE a, BULLET_TYPE b)
                {
                    return a < b;
                }); // BLANK group is always on top (for view only)

            // Debug show real bullet group
            debug_println("Bullet box:");
            for (const auto& item : Shotgun.listBullet)
            {
                debug_println(map_BULLET_TYPE[item]);
            }

            // Show sort bullet group
            for (uint8_t i = 0; i < Shotgun.listBulletImg.size(); i++)
            {
                if (i < bulletNum)
                {
                    // Bullet image
                    lv_image_set_src(Shotgun.listBulletImg[i], &Shotgun.mapBulletImg[listBulletSort[i]]);
                }
                else
                {
                    // Empty image
                    lv_image_set_src(Shotgun.listBulletImg[i], &Shotgun.mapBulletImg[BULLET_TYPE::MIN]);
                }
            }

            // Show message
            lv_obj_remove_flag(ui_lblCardMessage, LV_OBJ_FLAG_HIDDEN);
            lv_label_set_text(ui_lblCardMessage, MSG_TOUCH_SHOTGUN);

            HideBulletInTable();

            ResetPlayerTable();

            // Enable shotgun
            Shotgun.Enable();

            // Hide bullet box cover
            lv_obj_add_flag(ui_imgBulletBoxCover, LV_OBJ_FLAG_HIDDEN);

        }
        // Action play turn state
        else if (CurrentState.GetValue() == STATE_TYPE::ACTION_TURN)
        {
            if (CurrentState.GetOldValue() != STATE_TYPE::LOAD_SHELL)
            {
                // Move to the player who does not skip a turn
                while (true)
                {
                    Player = &player::NextPlayer(*Player);

                    if (Player->isSkip)
                    {
                        // Ignore this player now, next turn will be allowed
                        Player->isSkip = false;
                        continue;
                    }
                    else
                    {
                        break;
                    }
                }
            }
            else
            {
                // Move to latest player
                while (Player->type != latestPlayer)
                {
                    Player = &player::NextPlayer(*Player);
                }
            }

            debug_println("Player: " + map_PLAYER_TYPE[Player->type]);

            latestPlayer = Player->type;

            player::DisableAllPlayerTableExcept(*Player);
            Shotgun.RotateToPlayer(Player->angle);

            FSMTransit(STATE_TYPE::ACTION_ITEM);
        }
        // Action play item state
        else if (CurrentState.GetValue() == STATE_TYPE::ACTION_ITEM)
        {

        }
        // Update HP state
        else if (CurrentState.GetValue() == STATE_TYPE::UPDATE_HP)
        {
            player::UpdateAllPlayerHP();

            if (CurrentState.GetOldValue() == STATE_TYPE::SHOTGUN_SHOT)
            {
                // Transit previous state
                FSMTransit(STATE_TYPE::ACTION_ITEM);
            }
            else
            {
                // Transit previous state
                FSMTransit(CurrentState.GetOldValue());
            }
        }
    }

    CurrentState.ResetState();
}
