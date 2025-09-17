idf_component_register(SRCS
                           "main.c"
                           "../eve_lvgl_test.c"
                           "../test_font_montserrat_ascii_1bpp.c"
                           "../test_font_montserrat_ascii_2bpp.c"
                           "../test_font_montserrat_ascii_8bpp.c"
                           "../asset_watts_logo.c"
                           "../img_lvgl_logo.c"
                           "../img_bg.c"
                           "../img_bg_cut.c"
                       INCLUDE_DIRS
                           "."
                           "../components/lvgl"
                       REQUIRES
                           esp_driver_spi
                           esp_driver_gpio
                           esp_timer
)

# target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-error=uninitialized -Wno-error=format -Wno-error=maybe-uninitialized)
