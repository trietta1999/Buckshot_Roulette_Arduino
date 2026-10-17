#include <lvgl.h>
#include <Wire.h>
#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <lgfx/v1/platforms/esp32s3/Panel_RGB.hpp>
#include <lgfx/v1/platforms/esp32s3/Bus_RGB.hpp>

#include "src/ui/ui.h"
#include "src/CommonData.h"
#include "src/CommonLibrary.h"
#include "src/CommonService.h"

#define TFT_BL 2

#define TFT_HOR_RES 800
#define TFT_VER_RES 480

#define TOUCH_SDA 19
#define TOUCH_SCL 20
#define TOUCH_INT (-1)
#define TOUCH_RST 38

class LGFX : public lgfx::LGFX_Device { // Graphics configuration class
public:
  lgfx::Bus_RGB _bus_instance; // RGB bus instance
  lgfx::Panel_RGB _panel_instance; // RGB panel instance
  lgfx::Light_PWM _light_instance; // PWM backlight instance
  lgfx::Touch_GT911 _touch_instance; // GT911 touch instance

  LGFX(void) {
    {
      auto cfg = _panel_instance.config(); // Get panel config
      cfg.memory_width = TFT_HOR_RES; // Memory width
      cfg.memory_height = TFT_VER_RES; // Memory height
      cfg.panel_width = TFT_HOR_RES; // Display width
      cfg.panel_height = TFT_VER_RES; // Display height
      cfg.offset_x = 0; // X offset
      cfg.offset_y = 0; // Y offset
      _panel_instance.config(cfg); // Apply panel config
    }
    {
      auto cfg = _bus_instance.config(); // Get bus config
      cfg.panel = &_panel_instance; // Link panel

      cfg.pin_d0 = 8;   // B0 pin
      cfg.pin_d1 = 3;   // B1 pin
      cfg.pin_d2 = 46;  // B2 pin
      cfg.pin_d3 = 9;   // B3 pin
      cfg.pin_d4 = 1;   // B4 pin

      cfg.pin_d5 = 5;   // G0 pin
      cfg.pin_d6 = 6;   // G1 pin
      cfg.pin_d7 = 7;   // G2 pin
      cfg.pin_d8 = 15;  // G3 pin
      cfg.pin_d9 = 16;  // G4 pin
      cfg.pin_d10 = 4;  // G5 pin

      cfg.pin_d11 = 45;  // R0 pin
      cfg.pin_d12 = 48;  // R1 pin
      cfg.pin_d13 = 47;  // R2 pin
      cfg.pin_d14 = 21;  // R3 pin
      cfg.pin_d15 = 14;  // R4 pin

      cfg.pin_henable = 40; // DE pin
      cfg.pin_vsync = 41; // VSYNC pin
      cfg.pin_hsync = 39; // HSYNC pin
      cfg.pin_pclk = 42; // PCLK pin
      cfg.freq_write = 16000000; // Clock frequency

      cfg.hsync_polarity = 0; // HSync polarity
      cfg.hsync_back_porch = 8; // HSync back porch
      cfg.hsync_front_porch = 8; // HSync front porch
      cfg.hsync_pulse_width = 4; // HSync pulse width

      cfg.vsync_polarity = 0; // VSync polarity
      cfg.vsync_back_porch = 15; // VSync back porch
      cfg.vsync_front_porch = 8; // VSync front porch
      cfg.vsync_pulse_width = 4; // VSync pulse width

      cfg.pclk_active_neg = 1; // PCLK active edge
      cfg.de_idle_high = 1; // DE idle state
      cfg.pclk_idle_high = 1; // PCLK idle state

      _bus_instance.config(cfg); // Apply bus config
    }
    _panel_instance.setBus(&_bus_instance); // Bind bus to panel

    {
      auto cfg = _touch_instance.config(); // Get touch config
      cfg.x_min = 0; // Min X
      cfg.x_max = TFT_HOR_RES - 1; // Max X
      cfg.y_min = 0; // Min Y
      cfg.y_max = TFT_VER_RES - 1; // Max Y
      cfg.pin_int = TOUCH_INT; // Interrupt pin
      cfg.pin_rst = TOUCH_RST; // Reset pin
      cfg.bus_shared = false; // Dedicated bus
      cfg.i2c_port = I2C_NUM_1; // I2C port
      cfg.pin_sda = TOUCH_SDA; // SDA pin
      cfg.pin_scl = TOUCH_SCL; // SCL pin
      cfg.freq = 400000; // I2C frequency
      cfg.i2c_addr = 0x14; // GT911 address
      _touch_instance.config(cfg); // Apply touch config
      _panel_instance.setTouch(&_touch_instance); // Bind touch to panel
    }
    setPanel(&_panel_instance); // Set active panel
  }
};

static LGFX gfx; // Graphics device instance

// LVGL display flush callback
void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map) {
  uint32_t w = (area->x2 - area->x1 + 1); // Calculate width
  uint32_t h = (area->y2 - area->y1 + 1); // Calculate height

  if (gfx.getStartCount() == 0) {
    gfx.startWrite(); // Start SPI/Parallel transaction
  }

  gfx.pushImage(area->x1, area->y1, w, h, (uint16_t *)px_map); // Push pixels to display
  gfx.endWrite(); // End transaction

  lv_disp_flush_ready(disp); // Notify LVGL flush is done
}

// LVGL touchpad read callback
void my_touchpad_read(lv_indev_t *indev, lv_indev_data_t *data) {
  uint16_t touchX, touchY; // Temporary touch coordinates

  data->state = LV_INDEV_STATE_RELEASED; // Default state

  if (gfx.getTouch(&touchX, &touchY)) { // Check if screen is touched
    data->state = LV_INDEV_STATE_PRESSED; // Mark as pressed

    data->point.x = touchX; // Set X coordinate
    data->point.y = touchY; // Set Y coordinate
  } else
    data->state = LV_INDEV_STATE_RELEASED; // Mark as released
}

const uint32_t buf_size_in_bytes = TFT_HOR_RES * TFT_VER_RES * 2 / 10; // LVGL buffer size (1/10th of screen)
uint8_t *disp_draw_buf, *disp_draw_buf2; // LVGL draw buffers
lv_display_t *disp; // LVGL display object
lv_indev_t *indev; // LVGL input device object

void setup() {
  Serial.begin(115200); // Initialize serial for debugging

  gfx.begin(); // Initialize LovyanGFX
  gfx.setSwapBytes(true); // Swap bytes for correct color format
  lv_init(); // Initialize LVGL

#ifdef ESP32
  Serial.println("ESP32 capability draw buffer malloc()");
  disp_draw_buf = (uint8_t *)heap_caps_malloc(buf_size_in_bytes, MALLOC_CAP_DMA); // Allocate primary buffer in DMA RAM
  disp_draw_buf2 = (uint8_t *)heap_caps_malloc(buf_size_in_bytes, MALLOC_CAP_DMA); // Allocate secondary buffer in DMA RAM
#else
  Serial.println("Normal draw buffer malloc()");
  disp_draw_buf = (uint8_t *)malloc(buf_size_in_bytes); // Standard allocation
  disp_draw_buf2 = (uint8_t *)malloc(buf_size_in_bytes); // Standard allocation
#endif

  if (disp_draw_buf == nullptr) {
    Serial.println("LVGL disp_draw_buf allocate failed!");
    while (true)
    {
      delay(10); // Halt on failure
    }
  }

  if (disp_draw_buf2 == nullptr) {
    Serial.println("LVGL disp_draw_buf2 allocate failed - carry on anyway we can live without it");
  }

  disp = lv_display_create(TFT_HOR_RES, TFT_VER_RES); // Create LVGL display
  lv_display_set_flush_cb(disp, my_disp_flush); // Set flush callback
  lv_display_set_buffers(disp, disp_draw_buf, disp_draw_buf2, buf_size_in_bytes, LV_DISPLAY_RENDER_MODE_PARTIAL); // Register buffers

  indev = lv_indev_create(); // Create input device
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER); // Set as pointer type (touch)
  lv_indev_set_read_cb(indev, my_touchpad_read); // Set read callback

  ui_init(); // Initialize generated UI code

  InitData(); // Initialize application data

  Init(); // Initialize application logic/GUI state

  pinMode(TFT_BL, OUTPUT); // Set backlight pin as output
  analogWrite(TFT_BL, Brightness.GetValue()); // Set initial brightness

  Serial.println("Setup complete");
}

void IOProcessData() {
  // Update hardware based on software state
  if (Brightness.GetState()) { // If brightness state changed
    analogWrite(TFT_BL, Brightness.GetValue()); // Apply new brightness
  }
}

void loop() {
  lv_timer_handler(); // Process LVGL timers
  lv_task_handler(); // Process LVGL tasks
  CommonServiceProcess(); // Process background services (music, serial, etc.)
  IOProcessData(); // Handle hardware I/O
  AutoUpdate(); // Automatic state transitions
  UpdateAll(); // Refresh logic-driven data
  lv_tick_inc(10); // Increment LVGL internal tick
  delay(10); // Delay for task stability
}
