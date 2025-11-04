SET(SOURCES screens/ui_Main.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    ui_events.cpp
    images/ui_img_metal_table_png.c
    images/ui_img_empty_png.c
    images/ui_img_picking_png.c
    images/ui_img_block_png.c
    images/ui_img_chargeback_png.c
    images/ui_img_charge_png.c
    images/ui_img_shotgun_png.c
    images/ui_img_shotgun_block_png.c
    images/ui_img_metal_bullet_box_png.c
    images/ui_img_empty_bullet_png.c
    images/ui_img_hide_png.c
    images/ui_img_adrenaline_png.c
    images/ui_img_beer_png.c
    images/ui_img_blank_png.c
    images/ui_img_burnerphone_png.c
    images/ui_img_cigarette_png.c
    images/ui_img_expiredmedicine_png.c
    images/ui_img_handcuffs_png.c
    images/ui_img_handsaw_png.c
    images/ui_img_inverter_png.c
    images/ui_img_live_png.c
    images/ui_img_load_png.c
    images/ui_img_magnifyingglass_png.c
    images/ui_img_metal_turntable_png.c
    images/ui_img_shotgunback_png.c
    images/ui_img_shotguncut_png.c
    images/ui_img_shotgun_in_hand_png.c)

add_library(ui ${SOURCES})
