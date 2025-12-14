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
            debug_println_func("Change screen to Main");

            // Change screen to main screen
            _ui_screen_change(&ui_Main, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_Main_screen_init);

            FSMTransit(STATE_TYPE::PLAYER_NEXT);
        }
    }

    // Shotgun shot
    if (CurrentState.GetValue() == STATE_TYPE::SHOTGUN_SHOT)
    {
        if (MILLISEC_GET - lastShotgunTime >= EFFECT_WAIT_TIME)
        {
            debug_println_func("Shotgun shot stage " + std::to_string(Shotgun.state));

            BlockGui();

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
                    DelayCallback(MAKE_DELAY_CB{
                        FSMTransit(STATE_TYPE::PLAYER_NEXT);
                        }, nullptr, EFFECT_WAIT_TIME);
                }

                // Unblock all shotgun state
                for (int8_t i = Shotgun.state; i >= 0; i--)
                {
                    UnblockGui();
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
        debug_println_func("State: " + map_STATE_TYPE[CurrentState.GetValue()]);

        BlockGui();

        // Update player sequence
        if (CurrentState.GetValue() == STATE_TYPE::PLAYER_NEXT)
        {
            ResetPlayerTable();

            if (player::CheckAllPickComplete())
            {
                debug_println_func("All pick complete");

                Player->Enable();

                FSMTransit(STATE_TYPE::LOAD_SHELL);
            }
            // Move to next player
            else
            {
                debug_println_func("Next player");

                if (!Player)
                {
                    // Set player #1 as default
                    Player = &player::listPlayer[0];
                }
                else
                {
                    Player = &player::NextPlayer(*Player);
                }

                Player->Enable();

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
            Player->Enable();
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
            debug_println_func("Bullet box:");
            for (const auto& item : Shotgun.listBullet)
            {
                debug_println_func(map_BULLET_TYPE[item]);
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
            lv_label_set_text(ui_lblMessage, MSG_TOUCH_SHOTGUN);

            HideBulletInTable();

            ResetPlayerTable();

            // Enable shotgun
            Shotgun.Enable();

            // Reset bullet index
            Shotgun.currentBulletIndex = 0;

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

                    if (Player->isNext)
                    {
                        // Already to be the next turn
                        Player->isSkip = false;
                        Player->isNext = false;
                        break;
                    }
                    else if (Player->isSkip)
                    {
                        // Ignore this player now, next turn will be allowed
                        Player->isNext = true;
                        continue;
                    }
                    else
                    {
                        // Normal player
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

            debug_println_func("Player: " + map_PLAYER_TYPE[Player->type]);

            latestPlayer = Player->type;

            player::DisableAllPlayerTableExcept(*Player);
            Player->Enable();

            FSMTransit(STATE_TYPE::ACTION_ITEM);
        }
        // Action play item state
        else if (CurrentState.GetValue() == STATE_TYPE::ACTION_ITEM)
        {
            // Do nothing
        }
        // Update HP state
        else if (CurrentState.GetValue() == STATE_TYPE::UPDATE_HP)
        {
            player::UpdateAllPlayerHP();

            if (player::CheckAnyPlayerDead())
            {
                FSMTransit(STATE_TYPE::RESULT);
            }
            else
            {
                FSMTransit(STATE_TYPE::ACTION_ITEM);
            }
        }
        // Result state
        else if (CurrentState.GetValue() == STATE_TYPE::RESULT)
        {
            lv_obj_remove_flag(ui_wndResult, LV_OBJ_FLAG_HIDDEN); // Show result window

            // Player #1 is dead
            if (!player::listPlayer[0].hpLevel1)
            {
                lv_image_set_src(ui_imgPlayerLeftResult, &ui_img_skull_png);
            }
            // Player #2 is dead
            else if (!player::listPlayer[1].hpLevel1)
            {
                lv_image_set_src(ui_imgPlayerRightResult, &ui_img_skull_png);
            }
        }

        UnblockGui();
    }

    CurrentState.ResetState();
}
