#include "src/draw/eve/lv_eve.h"
#include "lvgl.h"
#include "../lvgl/examples/lv_examples.h"
#include "../lvgl/demos/lv_demos.h"

#include "esp_timer.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"

#define EVE_CS   GPIO_NUM_5
#define EVE_PDN  GPIO_NUM_4
#define EVE_SCK  GPIO_NUM_12
#define EVE_MISO GPIO_NUM_13
#define EVE_MOSI GPIO_NUM_11

static spi_device_handle_t spi_device = {0};

static void init_spi(void);
static void op_cb(lv_display_t * disp, lv_draw_eve_operation_t operation, void * data, uint32_t length);

static uint32_t tick_cb(void)
{
    int64_t micros = esp_timer_get_time();
    int64_t millis = micros / 1000;
    return millis;
}

// static void tim_cb(lv_timer_t * tim)
// {
//     static char buf[600];
//     static int x;
//     lv_memset(buf + x++ * 50, 'X', 50);
//     LV_LOG_USER("%d", x);
//     static lv_obj_t * obj;
//     if(!obj) {
//         obj = lv_label_create(lv_screen_active());
//         lv_label_set_long_mode(obj, LV_LABEL_LONG_MODE_WRAP);
//         lv_obj_set_width(obj, 200);
//         lv_obj_set_style_text_font(obj, &lv_font_montserrat_26, 0);
//         lv_obj_set_style_text_color(obj, lv_color_white(), 0);
//     }
//     lv_label_set_text_static(obj, buf);
// }

// static void tim_cb(lv_timer_t * tim)
// {
//     lv_obj_clean(lv_screen_active());
//     lv_obj_set_flex_flow(lv_screen_active(), LV_FLEX_FLOW_COLUMN_WRAP);

//     static int x;
//     x++;
//     for(int i = 0; i < x; i++) {
//         lv_obj_t * obj = lv_label_create(lv_screen_active());
//         lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, 0);
//         lv_obj_set_style_text_color(obj, lv_color_white(), 0);
//         lv_label_set_text_static(obj, "01234567890123456789");
//     }
// }

// static void anim_cb(void * arg, int32_t v);

// static void hamilton_1_item_part(lv_obj_t * parent, const char * top_text, const char * inner_text)
// {
//     lv_obj_t * label;

//     lv_obj_t * obj = lv_obj_create(parent);
//     lv_obj_remove_style_all(obj);
//     lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
//     lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);

//     label = lv_label_create(obj);
//     lv_obj_set_style_text_color(label, lv_color_white(), 0);
//     lv_label_set_text_static(label, top_text);

//     lv_obj_t * dd = lv_dropdown_create(obj);
//     lv_dropdown_set_text(dd, inner_text);
//     lv_obj_set_style_radius(dd, 0, 0);
//     lv_obj_set_style_border_width(dd, 0, 0);
// }


// static void hamilton_1_item(lv_obj_t * parent)
// {
//     lv_obj_t * label;

//     lv_obj_t * obj = lv_obj_create(parent);
//     lv_obj_remove_style_all(obj);
//     lv_obj_set_size(obj, LV_PCT(100), LV_SIZE_CONTENT);
//     lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
//     lv_obj_set_style_border_color(obj, lv_color_hex(0x242832), 0);
//     lv_obj_set_style_border_side(obj, LV_BORDER_SIDE_BOTTOM, 0);
//     lv_obj_set_style_border_opa(obj, LV_OPA_COVER, 0);
//     lv_obj_set_style_border_width(obj, 2, 0);
//     lv_obj_set_style_pad_ver(obj, 27, 0);

//     lv_obj_t * row0 = lv_obj_create(obj);
//     lv_obj_remove_style_all(row0);
//     lv_obj_set_flex_flow(row0, LV_FLEX_FLOW_ROW);
//     lv_obj_set_flex_align(row0, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
//     lv_obj_set_size(row0, LV_PCT(100), LV_SIZE_CONTENT);

//     label = lv_label_create(row0);
//     lv_obj_set_style_text_color(label, lv_color_white(), 0);
//     lv_label_set_text_static(label, "CH1");
//     lv_obj_set_style_border_width(label, 1, 0);
//     lv_obj_set_style_border_color(label, lv_color_hex(0x0ab6ff), 0);
//     lv_obj_set_style_radius(label, LV_COORD_MAX, 0);
//     lv_obj_set_size(label, 30, 30);

//     label = lv_label_create(row0);
//     lv_obj_set_style_text_color(label, lv_color_white(), 0);
//     lv_label_set_text_static(label, "CHANNEL 1");

//     label = lv_label_create(row0);
//     lv_obj_set_style_text_color(label, lv_color_white(), 0);
//     lv_label_set_text_static(label, "Input Required");

//     LV_IMAGE_DECLARE(img_clothes);
//     lv_image_set_src(lv_image_create(row0), &img_clothes);

//     label = lv_label_create(row0);
//     lv_label_set_text_static(label, "CALIBRATE");
//     lv_obj_set_style_bg_opa(label, LV_OPA_COVER, 0);
//     lv_obj_set_style_bg_color(label, lv_color_hex(0x144d77), 0);

//     label = lv_label_create(row0);
//     lv_obj_set_style_text_color(label, lv_color_white(), 0);
//     lv_label_set_text_static(label, "START");

//     lv_obj_t * play = lv_image_create(row0);
//     lv_image_set_src(play, LV_SYMBOL_PLAY);
//     lv_obj_set_style_recolor_opa(play, LV_OPA_COVER, 0);
//     lv_obj_set_style_recolor(play, lv_color_white(), 0);
//     lv_obj_set_style_radius(play, LV_COORD_MAX, 0);
//     lv_obj_set_style_border_width(play, 1, 0);
//     lv_obj_set_style_border_color(play, lv_color_white(), 0);
//     lv_obj_set_size(play, 45, 45);

//     lv_obj_t * row1 = lv_obj_create(obj);
//     lv_obj_remove_style_all(row1);
//     lv_obj_set_flex_flow(row1, LV_FLEX_FLOW_ROW);
//     lv_obj_set_flex_align(row1, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_SPACE_EVENLY);
//     lv_obj_set_size(row1, LV_PCT(100), LV_SIZE_CONTENT);

//     hamilton_1_item_part(row1, "Syringe", "Select");
//     hamilton_1_item_part(row1, "Mode", "Select");
//     hamilton_1_item_part(row1, "Start Delay", "Edit");
//     hamilton_1_item_part(row1, "Volume", "Edit");
//     hamilton_1_item_part(row1, "Time/Flow", "Edit");

//     lv_obj_t * row2 = lv_obj_create(obj);
//     lv_obj_remove_style_all(row2);
//     lv_obj_set_flex_flow(row2, LV_FLEX_FLOW_ROW);
//     lv_obj_set_flex_align(row2, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
//     lv_obj_set_size(row2, LV_PCT(100), LV_SIZE_CONTENT);

//     label = lv_label_create(row2);
//     lv_obj_set_style_text_color(label, lv_color_white(), 0);
//     lv_label_set_text_static(label, "Flow rate");

//     label = lv_label_create(row2);
//     lv_obj_set_style_text_color(label, lv_color_white(), 0);
//     lv_label_set_text_static(label, "N/A");

//     label = lv_label_create(row2);
//     lv_obj_set_style_text_color(label, lv_color_white(), 0);
//     lv_label_set_text_static(label, "Remaining Time");

//     label = lv_label_create(row2);
//     lv_obj_set_style_text_color(label, lv_color_white(), 0);
//     lv_label_set_text_static(label, "N/A");

//     label = lv_label_create(row2);
//     lv_obj_set_style_text_color(label, lv_color_white(), 0);
//     lv_label_set_text_static(label, "Remaining Volume");

//     label = lv_label_create(row2);
//     lv_obj_set_style_text_color(label, lv_color_white(), 0);
//     lv_label_set_text_static(label, "N/A");
// }

// static lv_obj_t * hamilton_base(void)
// {
//     lv_obj_t * scr = lv_screen_active();
//     lv_obj_t * label;

//     lv_obj_set_style_bg_color(scr, lv_color_hex(0x0c101c), 0);

//     lv_obj_set_flex_flow(scr, LV_FLEX_FLOW_COLUMN);

//     lv_obj_t * top_bar = lv_obj_create(scr);
//     lv_obj_remove_style_all(top_bar);
//     lv_obj_set_size(top_bar, LV_PCT(100), LV_SIZE_CONTENT);
//     lv_obj_set_flex_flow(top_bar, LV_FLEX_FLOW_ROW);
//     lv_obj_set_style_border_color(top_bar, lv_color_hex(0x184012), 0);
//     lv_obj_set_style_border_side(top_bar, LV_BORDER_SIDE_TOP, 0);
//     lv_obj_set_style_border_opa(top_bar, LV_OPA_COVER, 0);
//     lv_obj_set_style_border_width(top_bar, 5, 0);

//     LV_IMAGE_DECLARE(img_clothes);
//     lv_image_set_src(lv_image_create(top_bar), &img_clothes);
//     lv_image_set_src(lv_image_create(top_bar), &img_clothes);
//     lv_image_set_src(lv_image_create(top_bar), &img_clothes);
//     lv_image_set_src(lv_image_create(top_bar), &img_clothes);
//     label = lv_label_create(top_bar);
//     lv_obj_set_style_text_color(label, lv_color_white(), 0);
//     lv_label_set_text_static(label, "10:00 a.m. / 01.05.2025");
//     lv_image_set_src(lv_image_create(top_bar), &img_clothes);

//     lv_obj_t * under_bar = lv_obj_create(scr);
//     lv_obj_remove_style_all(under_bar);
//     lv_obj_set_flex_grow(under_bar, 1);
//     lv_obj_set_width(under_bar, LV_PCT(100));
//     lv_obj_set_flex_flow(under_bar, LV_FLEX_FLOW_ROW);
//     lv_obj_set_style_border_color(under_bar, lv_color_hex(0x1c2d57), 0);
//     lv_obj_set_style_border_side(under_bar, LV_BORDER_SIDE_TOP, 0);
//     lv_obj_set_style_border_opa(under_bar, LV_OPA_COVER, 0);
//     lv_obj_set_style_border_width(under_bar, 2, 0);

//     lv_obj_t * left = lv_obj_create(under_bar);
//     lv_obj_remove_style_all(left);
//     lv_obj_set_size(left, 150, LV_PCT(100));
//     lv_obj_set_style_bg_opa(left, LV_OPA_COVER, 0);
//     lv_obj_set_style_bg_color(left, lv_color_hex(0x1c2d57), 0);

//     lv_obj_t * left_selected = lv_obj_create(left);
//     lv_obj_remove_style_all(left_selected);
//     lv_obj_set_style_bg_opa(left_selected, LV_OPA_COVER, 0);
//     lv_obj_set_style_bg_color(left_selected, lv_color_hex(0x0c101c), 0);
//     lv_obj_set_size(left_selected, LV_PCT(100), 65);
//     lv_obj_set_y(left_selected, 65);
//     lv_obj_set_style_border_color(left_selected, lv_color_hex(0x00f091), 0);
//     lv_obj_set_style_border_side(left_selected, LV_BORDER_SIDE_LEFT, 0);
//     lv_obj_set_style_border_opa(left_selected, LV_OPA_COVER, 0);
//     lv_obj_set_style_border_width(left_selected, 5, 0);

//     label = lv_label_create(left);
//     lv_obj_set_style_text_color(label, lv_color_white(), 0);
//     lv_label_set_text_static(label, "HOME");
//     lv_obj_set_pos(label, 20, 30);

//     label = lv_label_create(left);
//     lv_obj_set_style_text_color(label, lv_color_white(), 0);
//     lv_label_set_text_static(label, "RUN");
//     lv_obj_set_pos(label, 20, 90);

//     label = lv_label_create(left);
//     lv_obj_set_style_text_color(label, lv_color_white(), 0);
//     lv_label_set_text_static(label, "SETTINGS");
//     lv_obj_set_pos(label, 20, 150);

//     label = lv_label_create(left);
//     lv_obj_set_style_text_color(label, lv_color_white(), 0);
//     lv_label_set_text_static(label, "ADMIN");
//     lv_obj_align(label, LV_ALIGN_BOTTOM_LEFT, 20, -30);

//     lv_obj_t * main_area = lv_obj_create(under_bar);
//     lv_obj_remove_style_all(main_area);
//     lv_obj_set_flex_grow(main_area, 1);
//     lv_obj_set_height(main_area, LV_PCT(100));
//     lv_obj_set_flex_flow(main_area, LV_FLEX_FLOW_COLUMN);

//     return main_area;
// }


// static void hamilton_1(void)
// {
//     lv_obj_t * main_area = hamilton_base();

//     hamilton_1_item(main_area);
//     // hamilton_1_item(main_area);
//     // hamilton_1_item(main_area);
// }


// static void hamilton_2_row(lv_obj_t * main_area)
// {
//     lv_obj_t * label;
//     lv_obj_t * row;

//     row = lv_obj_create(main_area);
//     lv_obj_remove_style_all(row);
//     lv_obj_set_flex_flow(row, LV_FLEX_FLOW_ROW);
//     lv_obj_set_flex_align(row, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
//     lv_obj_set_size(row, LV_PCT(100), LV_SIZE_CONTENT);
//     lv_obj_set_style_border_color(row, lv_color_hex(0x242832), 0);
//     lv_obj_set_style_border_side(row, LV_BORDER_SIDE_BOTTOM, 0);
//     lv_obj_set_style_border_opa(row, LV_OPA_COVER, 0);
//     lv_obj_set_style_border_width(row, 2, 0);

//     label = lv_label_create(row);
//     lv_obj_set_style_text_color(label, lv_color_white(), 0);
//     lv_label_set_text_static(label, "Infusion, Ch2, 35 mins");

//     label = lv_label_create(row);
//     lv_obj_set_style_text_color(label, lv_color_white(), 0);
//     lv_label_set_text_static(label, "22.10.2024 | 14:44:19");
// }


// static void hamilton_2(void)
// {
//     lv_obj_t * main_area = hamilton_base();
//     lv_obj_t * label;
//     lv_obj_t * row;


//     row = lv_obj_create(main_area);
//     lv_obj_remove_style_all(row);
//     lv_obj_set_flex_flow(row, LV_FLEX_FLOW_ROW);
//     lv_obj_set_flex_align(row, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
//     lv_obj_set_size(row, LV_PCT(100), LV_SIZE_CONTENT);

//     LV_IMAGE_DECLARE(img_clothes);
//     lv_image_set_src(lv_image_create(row), &img_clothes);

//     label = lv_label_create(row);
//     lv_obj_set_style_text_color(label, lv_color_white(), 0);
//     lv_label_set_text_static(label, "SETTINGS");

//     label = lv_label_create(row);
//     lv_obj_set_style_text_color(label, lv_color_white(), 0);
//     lv_label_set_text_static(label, " / VIEW LOGS");


//     row = lv_obj_create(main_area);
//     lv_obj_remove_style_all(row);
//     lv_obj_set_flex_flow(row, LV_FLEX_FLOW_ROW);
//     lv_obj_set_flex_align(row, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
//     lv_obj_set_size(row, LV_PCT(100), LV_SIZE_CONTENT);
//     lv_obj_set_style_border_color(row, lv_color_hex(0x242832), 0);
//     lv_obj_set_style_border_side(row, LV_BORDER_SIDE_BOTTOM, 0);
//     lv_obj_set_style_border_opa(row, LV_OPA_COVER, 0);
//     lv_obj_set_style_border_width(row, 2, 0);

//     label = lv_label_create(row);
//     lv_obj_set_style_text_color(label, lv_color_white(), 0);
//     lv_label_set_text_static(label, "Logs");

//     label = lv_label_create(row);
//     lv_obj_set_style_text_color(label, lv_color_white(), 0);
//     lv_label_set_text_static(label, "Timestamp");

//     lv_image_set_src(lv_image_create(row), &img_clothes);
//     lv_image_set_src(lv_image_create(row), &img_clothes);

//     hamilton_2_row(main_area);
//     hamilton_2_row(main_area);
//     hamilton_2_row(main_area);
//     hamilton_2_row(main_area);
//     hamilton_2_row(main_area);

//     row = lv_obj_create(main_area);
//     lv_obj_remove_style_all(row);
//     lv_obj_set_flex_flow(row, LV_FLEX_FLOW_ROW);
//     lv_obj_set_flex_align(row, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
//     lv_obj_set_size(row, LV_PCT(100), LV_SIZE_CONTENT);

//     label = lv_label_create(row);
//     lv_obj_set_style_text_color(label, lv_color_white(), 0);
//     lv_label_set_text_static(label, "Items per page");

//     lv_obj_t * dd = lv_dropdown_create(row);
//     lv_dropdown_set_text(dd, "5");

//     label = lv_label_create(row);
//     lv_obj_set_style_text_color(label, lv_color_white(), 0);
//     lv_label_set_text_static(label, "1 - 5 of 10");

//     lv_image_set_src(lv_image_create(row), &img_clothes);
//     lv_image_set_src(lv_image_create(row), &img_clothes);
//     lv_image_set_src(lv_image_create(row), &img_clothes);
//     lv_image_set_src(lv_image_create(row), &img_clothes);

//     label = lv_label_create(row);
//     lv_obj_set_style_text_color(label, lv_color_white(), 0);
//     lv_label_set_text_static(label, "EXPORT");
//     lv_obj_set_style_border_width(label, 1, 0);
//     lv_obj_set_style_border_color(label, lv_color_hex(0x0ab6ff), 0);
//     lv_obj_set_style_radius(label, LV_COORD_MAX, 0);
//     lv_obj_set_size(label, 100, 40);
// }

// static void tim_cb(lv_timer_t * tim)
// {
//     static unsigned rot;
//     lv_display_set_rotation(lv_timer_get_user_data(tim), rot++ % 4);
// }

static void arc_issue(void)
{
    lv_obj_set_style_bg_color(lv_screen_active(), lv_color_black(), 0);

    lv_obj_set_flex_flow(lv_screen_active(), LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_style_pad_right(lv_screen_active(), 500, 0);

    LV_IMAGE_DECLARE(img_lvgl_logo);
    
    for(int i = 0; i < 20; i++) {
        lv_obj_t * img = lv_image_create(lv_screen_active());
        lv_image_set_src(img, &img_lvgl_logo);
    }

    lv_obj_t * arc = lv_arc_create(lv_screen_active());
    lv_obj_add_flag(arc, LV_OBJ_FLAG_IGNORE_LAYOUT);
    // lv_obj_center(arc);
    lv_obj_set_style_bg_opa(arc, 0, LV_PART_KNOB);
    lv_obj_set_style_arc_opa(arc, 0, 0);
    
    lv_obj_set_size(arc, 200, 200);
    lv_obj_set_style_arc_width(arc, 40, LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(arc, lv_color_white(), LV_PART_INDICATOR);
    lv_obj_set_style_arc_opa(arc, 127, LV_PART_INDICATOR);
}

void entrypoint()
{
    lv_init(); /* LVGL Init */
    lv_tick_set_cb(tick_cb);
 
    init_spi();

    lv_draw_eve_parameters_t params = {
        .hor_res = 800,
        .ver_res = 480,
        .vsync0 = 0,
        .vsync1 = 4,
        .voffset = 8,
        .vcycle = 496,
        .hsync0 = 0,
        .hsync1 = 4,
        .hoffset = 8,
        .hcycle = 816,
        .pclk = 3,
        .pclkpol = 1,
        .swizzle = 0,
        .cspread = 0,
        .has_crystal = true,
        .has_gt911 = false,
        .backlight_freq = 4000,
        .backlight_pwm = 128,
    };

    lv_display_t * disp = lv_draw_eve_display_create(&params, op_cb, NULL);
    // lv_draw_eve_touch_create(disp);

    // lv_display_set_rotation(disp, LV_DISPLAY_ROTATION_270);
    // lv_timer_create(tim_cb, 1000, disp);

    // LV_IMAGE_DECLARE(img_bg);
    // lv_obj_t * img = lv_image_create(lv_screen_active());
    // lv_image_set_src(img, &img_bg);


    lv_demo_widgets();
    lv_demo_widgets_start_slideshow();

    // arc_issue();


    // lv_demo_benchmark();

    // lv_obj_set_style_bg_color(lv_screen_active(), lv_palette_main(LV_PALETTE_BLUE), 0);
    // LV_IMAGE_DECLARE(img_demo_widgets_avatar);
    // LV_IMAGE_DECLARE(asset_watts_logo);
    // LV_IMAGE_DECLARE(img_benchmark_lvgl_logo_rgb);
    // lv_image_set_src(lv_image_create(lv_screen_active()), &img_benchmark_lvgl_logo_rgb);

    // lv_sysmon_hide_performance(NULL);

    // lv_obj_t * obj = lv_image_create(lv_screen_active());
    // LV_IMAGE_DECLARE(img_benchmark_lvgl_logo_argb);
    // lv_image_set_src(obj, &img_benchmark_lvgl_logo_argb);
    // lv_image_set_rotation(obj, 300);
    // lv_obj_set_style_outline_width(obj, 2, 0);
    // lv_obj_set_style_radius(obj, 0, 0);
    // lv_obj_set_style_outline_color(obj, lv_color_black(), 0);
    // lv_obj_set_style_outline_opa(obj, 255, 0);

    // lv_anim_t a;
    // lv_anim_init(&a);
    // lv_anim_set_exec_cb(&a, anim_cb);
    // lv_anim_set_values(&a, 0, 1000);
    // lv_anim_set_var(&a, obj);
    // lv_anim_set_duration(&a, 3000);
    // lv_anim_set_reverse_duration(&a, 3000);
    // lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    // lv_anim_start(&a);


    // lv_timer_t * tim = lv_timer_create(tim_cb, 500, NULL);
    // lv_timer_set_repeat_count(tim, 10);



    // hamilton_1();


    while (1)
    {
        lv_timer_handler();
    }
}

// static void anim_cb(void * arg, int32_t v)
// {
//     // lv_obj_set_pos(arg, lv_map(v, 0, 1000, 0, 800 - 101), lv_map(v, 0, 1000, 0, 480 - 101));
//     lv_obj_set_pos(arg, lv_map(v, 0, 1000, 0, (800 - 101) / 10), lv_map(v, 0, 1000, 0, (480 - 101) / 10));
// }

static void init_spi(void)
{
    spi_bus_config_t buscfg = {0};
    spi_device_interface_config_t devcfg = {0};
    gpio_config_t io_cfg = {0};

    io_cfg.mode = GPIO_MODE_OUTPUT;
    io_cfg.pin_bit_mask = BIT64(EVE_PDN) | BIT64(EVE_CS);
    gpio_config(&io_cfg);

    gpio_set_level(EVE_CS, 1);
    gpio_set_level(EVE_PDN, 0);

    buscfg.mosi_io_num = EVE_MOSI;
    buscfg.miso_io_num = EVE_MISO;
    buscfg.sclk_io_num = EVE_SCK;
    buscfg.quadwp_io_num = -1;
    buscfg.quadhd_io_num = -1;
    buscfg.max_transfer_sz= 4088;

    devcfg.clock_speed_hz = 20U * 1000U * 1000U; /* clock = 10 MHz */
    devcfg.mode = 0;                          /* SPI mode 0 */
    devcfg.spics_io_num = -1;                 /* CS pin operated by app */
    devcfg.queue_size = 3;                    /* we need only one transaction in the que */
    devcfg.command_bits = 0;                  /* command operated by app */
    devcfg.address_bits = 0;
    devcfg.post_cb = 0;

    spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO);
    spi_bus_add_device(SPI2_HOST, &devcfg, &spi_device);
}

static void op_cb(lv_display_t * disp, lv_draw_eve_operation_t operation, void * data, uint32_t length)
{
    switch(operation) {
        case LV_DRAW_EVE_OPERATION_POWERDOWN_SET:
            gpio_set_level(EVE_PDN, 0);
            break;
        case LV_DRAW_EVE_OPERATION_POWERDOWN_CLEAR:
            gpio_set_level(EVE_PDN, 1);
            break;
        case LV_DRAW_EVE_OPERATION_CS_ASSERT:
            spi_device_acquire_bus(spi_device, portMAX_DELAY);
            gpio_set_level(EVE_CS, 0U);
            break;
        case LV_DRAW_EVE_OPERATION_CS_DEASSERT:
            gpio_set_level(EVE_CS, 1U);
            spi_device_release_bus(spi_device);
            break;
        case LV_DRAW_EVE_OPERATION_SPI_SEND: {
                spi_transaction_t trans = {0};
                trans.length = 8U * length;
                trans.tx_buffer = data;
                spi_device_polling_transmit(spi_device, &trans);
            }
            break;
        case LV_DRAW_EVE_OPERATION_SPI_RECEIVE: {
                spi_transaction_t trans = {0};
                trans.length = trans.rxlength = 8U * length;
                trans.rx_buffer = data;
                spi_device_polling_transmit(spi_device, &trans);
            }
            break;
    }
}
