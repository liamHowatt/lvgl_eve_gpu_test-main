idf_component_register(SRCS
                           "main.c"
                           "../tft_eve_init.c"
                           "../eve_lvgl_test.c"
                           "../FT800-FT813/EVE_commands.c"
                           "../FT800-FT813/EVE_supplemental.c"
                           "../FT800-FT813/EVE_target.c"
                       INCLUDE_DIRS
                           "."
                           "../FT800-FT813"
                           "../components/lvgl"
                       REQUIRES
                           esp_driver_spi
                           esp_driver_gpio
                           esp_timer
)

target_compile_definitions(${COMPONENT_LIB} PUBLIC "-DEVE_RVT50H")
target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-error=uninitialized -Wno-error=format -Wno-error=maybe-uninitialized)
