#pragma once

/**
 * @brief Finite State Machine (FSM) implementation for game logic
 */

// Transitions to a new state and immediately processes it if changed
void FSMTransit(STATE_TYPE next)
{
    CurrentState.SetValue(next);

    if (CurrentState.GetState())
    {
        FSM();
    }
}

// Main FSM processor - handles game state transitions and logic
void FSM()
{
#pragma region Common_state
    // STARTUP: Initial screen where both players must 'check-in'
    if (CurrentState.GetValue() == STATE_TYPE::STARTUP)
    {
        if (CheckObjectState(ui_btnPlayer1Start, LV_STATE_CHECKED) && CheckObjectState(ui_btnPlayer2Start, LV_STATE_CHECKED))
        {
            debug_println_func("Change screen to Main");

            // Change screen to main game interface
            _ui_screen_change(&ui_Main, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_Main_screen_init);

            FSMTransit(STATE_TYPE::PLAYER_NEXT);
        }
    }

    // SHOTGUN_SHOT: Sequence of firing the shotgun with timing for effects
    if (CurrentState.GetValue() == STATE_TYPE::SHOTGUN_SHOT)
    {
        if (MILLISEC_GET - lastShotgunTime >= EFFECT_WAIT_TIME)
        {
            debug_println_func("Shotgun shot stage " + std::to_string(Shotgun.state));

            BlockGui(); // Block interaction during shot animation

            // Shot animation sequence finished (multiple stages for recoil/sound/smoke)
            if (Shotgun.state > 2)
            {
                FSMTransit(STATE_TYPE::UPDATE_HP);

                // If gun fired (Live) or user shot themselves (Blank and target is self), end turn
                if ((Shotgun.isGunfire) || (Player != Shotgun.targetPlayer))
                {
                    FSMTransit(STATE_TYPE::ACTION_TURN);
                }

                ShowTrashBullet(); // Display discarded shell

                Shotgun.Reset();

                // If current magazine is empty, prepare next round loading
                if (!Shotgun.queueBullet.size())
                {
                    // Wait for transit to next round
                    DelayCallback(MAKE_DELAY_CB{
                        FSMTransit(STATE_TYPE::PLAYER_NEXT);
                        }, nullptr, EFFECT_WAIT_TIME);
                }

                // Release GUI block for all stages of shot
                for (int8_t i = Shotgun.state; i >= 0; i--)
                {
                    UnblockGui();
                }
            }

            Shotgun.Shot(); // Execute current shot effect stage
            Shotgun.state++;

            lastShotgunTime = MILLISEC_GET;
        }
    }
#pragma endregion

    // Process only if the state was just set
    if (CurrentState.GetState())
    {
        debug_println_func("State: " + map_STATE_TYPE[CurrentState.GetValue()]);

        BlockGui(); // Standard block for all major state transitions

        // PLAYER_NEXT: Decides whether to load more shells or let next player pick items
        if (CurrentState.GetValue() == STATE_TYPE::PLAYER_NEXT)
        {
            ResetPlayerTable();

            if (player::CheckAllPickComplete()) // If all players have picked their round items
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
                    Player = &player::listPlayer[0]; // Start with player 1
                }
                else
                {
                    Player = &player::NextPlayer(*Player); // Toggle players
                }

                Player->Enable();

                FSMTransit(STATE_TYPE::PICK);
            }
        }
        // PICK: Waiting for player to pick a random item from the box
        else if (CurrentState.GetValue() == STATE_TYPE::PICK)
        {
            Player->EnablePickButton();
        }
        // PLAYER_ITEM_ARRANGE: Internal state for organizing inventory UI
        else if (CurrentState.GetValue() == STATE_TYPE::PLAYER_ITEM_ARRANGE)
        {
            Player->Enable();
        }
        // LOAD_SHELL: Shotgun loading sequence with randomized Live/Blank count
        else if (CurrentState.GetValue() == STATE_TYPE::LOAD_SHELL)
        {
            uint8_t bulletNum = RandomRangeNumber(MIN_BULLET, MAX_BULLET);

            Shotgun.listBullet = CreateBulletList(bulletNum);
            Shotgun.VecToQueue(Shotgun.listBullet);

            // Sort for the 'reveal' UI so Blanks are shown together
            std::vector<BULLET_TYPE> listBulletSort = Shotgun.listBullet;
            std::sort(listBulletSort.begin(), listBulletSort.end(), [](BULLET_TYPE a, BULLET_TYPE b)
                {
                    return a < b;
                });

            // Debug show real bullet group
            debug_println_func("Bullet box:");
            for (const auto& item : Shotgun.listBullet)
            {
                debug_println_func(map_BULLET_TYPE[item]);
            }

            // Update bullet revelation UI
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

            lv_obj_add_flag(ui_imgBulletBoxCover, LV_OBJ_FLAG_HIDDEN); // Show bullets
        }
        // ACTION_TURN: Logic for switching active turns based on skip states (handcuffs)
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
                        Player->SetSkip(false);
                        Player->isNext = false;

                        break;
                    }
                    else if (Player->isSkip)
                    {
                        Player->isNext = true; // Mark to be allowed next round


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
                // Ensure we stick to the latest player who should move after loading
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
        // ACTION_ITEM: Waiting for player interaction with shotgun or inventory items
        else if (CurrentState.GetValue() == STATE_TYPE::ACTION_ITEM)
        {
            // Passive wait for user action
        }
        // UPDATE_HP: Refresh UI health levels and check for game over
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
        // RESULT: Show winner/loser and end game
        else if (CurrentState.GetValue() == STATE_TYPE::RESULT)
        {
            lv_obj_remove_flag(ui_wndResult, LV_OBJ_FLAG_HIDDEN);

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

    CurrentState.ResetState(); // Clear state flag to prevent loops
}
