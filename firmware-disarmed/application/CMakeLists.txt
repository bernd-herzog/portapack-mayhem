#
# Copyright (C) 2014 Jared Boone, ShareBrained Technology, Inc.
# Copyright (C) 2016 Furrtek
#
# This file is part of PortaPack.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
#

##############################################################################
# Build global options
# NOTE: Can be overridden externally.
#

enable_language(C CXX ASM)

project(application-disarmed)

# Compiler options here.
set(USE_OPT "-Os -g --specs=nano.specs --specs=nosys.specs")

# C specific options here (added to USE_OPT).
set(USE_COPT "-std=gnu99")

# C++ specific options here (added to USE_OPT).
set(USE_CPPOPT "-std=c++17 -fno-rtti -fno-exceptions -Weffc++ -Wuninitialized")

# Enable this if you want the linker to remove unused code and data
set(USE_LINK_GC yes)

# Linker extra options here.
set(USE_LDOPT)

# Enable this if you want link time optimizations (LTO)
set(USE_LTO no)

# If enabled, this option allows to compile the application in THUMB mode.
set(USE_THUMB yes)

# Enable this if you want to see the full log while compiling.
set(USE_VERBOSE_COMPILE no)

#
# Build global options
##############################################################################

##############################################################################
# Architecture or project specific options
#

# Enables the use of FPU on Cortex-M4 (no, softfp, hard).
set(USE_FPU no)

#
# Architecture or project specific options
##############################################################################

##############################################################################
# Project, sources and paths
#

set(CPLD_20150901_SVF_PATH ${HARDWARE_PATH}/portapack_h1/cpld/20150901/output_files/portapack_h1_cpld.svf)
set(CPLD_20150901_DATA_CPP ${CMAKE_CURRENT_BINARY_DIR}/portapack_cpld_20150901_data.cpp)

set(CPLD_20170522_SVF_PATH ${HARDWARE_PATH}/portapack_h1/cpld/20170522/output_files/portapack_h1_cpld.svf)
set(CPLD_20170522_DATA_CPP ${CMAKE_CURRENT_BINARY_DIR}/portapack_cpld_20170522_data.cpp)

set(HACKRF_CPLD_DATA_HPP ${CMAKE_CURRENT_BINARY_DIR}/hackrf_cpld_data.hpp)
set(HACKRF_CPLD_DATA_CPP ${CMAKE_CURRENT_BINARY_DIR}/hackrf_cpld_data.cpp)

# Imported source files and paths
include(${CHIBIOS_PORTAPACK}/boards/PORTAPACK_APPLICATION/board.cmake)
include(${CHIBIOS_PORTAPACK}/os/hal/platforms/LPC43xx_M0/platform.cmake)
include(${CHIBIOS}/os/hal/hal.cmake)
include(${CHIBIOS_PORTAPACK}/os/ports/GCC/ARMCMx/LPC43xx_M0/port.cmake)
include(${CHIBIOS}/os/kernel/kernel.cmake)
include(${CHIBIOS_PORTAPACK}/os/various/fatfs_bindings/fatfs.cmake)
include(${CHIBIOS}/test/test.cmake)

# Define linker script file here
set(LDSCRIPT ${PORTLD}/LPC43xx_M0.ld)

set(FIRMWARE_APPLICATION ${FIRMWARE_SOURCE_DIR}/application)

# C sources that can be compiled in ARM or THUMB mode depending on the global
# setting.
set(CSRC
	${PORTSRC}
	${KERNSRC}
	${TESTSRC}
	${HALSRC}
	${PLATFORMSRC}
	${BOARDSRC}
	${FATFSSRC}
	${FIRMWARE_APPLICATION}/firmware_info.c
	tiny_memory_elua/lapi.c
	tiny_memory_elua/lcode.c
	#tiny_memory_elua/lctype.c
	tiny_memory_elua/ldebug.c
	tiny_memory_elua/ldo.c
	tiny_memory_elua/ldump.c
	tiny_memory_elua/lfunc.c
	tiny_memory_elua/lgc.c
	tiny_memory_elua/llex.c
	tiny_memory_elua/lmem.c
	tiny_memory_elua/lobject.c
	tiny_memory_elua/lopcodes.c
	tiny_memory_elua/lparser.c
	tiny_memory_elua/lstate.c
	tiny_memory_elua/lstring.c
	tiny_memory_elua/ltable.c
	tiny_memory_elua/ltm.c
	tiny_memory_elua/lundump.c
	tiny_memory_elua/lvm.c
	tiny_memory_elua/lzio.c
	tiny_memory_elua/lauxlib.c
	tiny_memory_elua/lbaselib.c
	#tiny_memory_elua/lcorolib.c
	tiny_memory_elua/legc.c
	tiny_memory_elua/lrotable.c
	tiny_memory_elua/ldblib.c
	#tiny_memory_elua/liolib.c // enable for file access
	tiny_memory_elua/lmathlib.c
	tiny_memory_elua/loadlib.c
	tiny_memory_elua/loslib.c
	tiny_memory_elua/lstrlib.c
	tiny_memory_elua/ltablib.c
	#tiny_memory_elua/lutf8lib.c
	tiny_memory_elua/linit.c
)

# C++ sources that can be compiled in ARM or THUMB mode depending on the global
# setting.
set(CPPSRC
	main.cpp
	${COMMON}/acars_packet.cpp
	${COMMON}/adsb.cpp
	${COMMON}/adsb_frame.cpp
	${COMMON}/ais_baseband.cpp
	${COMMON}/ais_packet.cpp
	${COMMON}/ak4951.cpp
	${COMMON}/backlight.cpp
	${COMMON}/baseband_cpld.cpp
	${COMMON}/bch_code.cpp
	${COMMON}/buffer.cpp
	${COMMON}/buffer_exchange.cpp
	${COMMON}/chibios_cpp.cpp
	${COMMON}/cpld_max5.cpp
	${COMMON}/cpld_update.cpp
	${COMMON}/cpld_xilinx.cpp
	${FIRMWARE_APPLICATION}/debug.cpp
	${COMMON}/ert_packet.cpp
	${COMMON}/event.cpp
	${COMMON}/gcc.cpp
	${COMMON}/hackrf_hal.cpp
	${COMMON}/i2c_pp.cpp
	${COMMON}/jtag.cpp
	${COMMON}/jtag_tap.cpp
	${COMMON}/lcd_ili9341.cpp
	${COMMON}/lfsr_random.cpp
	${COMMON}/manchester.cpp
	${COMMON}/message_queue.cpp
	${COMMON}/morse.cpp
	${COMMON}/png_writer.cpp
	${COMMON}/pocsag.cpp
	${COMMON}/pocsag_packet.cpp
	${COMMON}/aprs_packet.cpp
	${COMMON}/portapack_io.cpp
	${COMMON}/portapack_persistent_memory.cpp
	${COMMON}/portapack_shared_memory.cpp
	${COMMON}/sonde_packet.cpp
	# ${COMMON}/test_packet.cpp
	${COMMON}/tpms_packet.cpp
	${COMMON}/ui.cpp
	${COMMON}/ui_focus.cpp
	${COMMON}/ui_painter.cpp
	${COMMON}/ui_text.cpp
	${COMMON}/ui_widget.cpp
	${COMMON}/utility.cpp
	${COMMON}/wm8731.cpp
	${FIRMWARE_APPLICATION}/app_settings.cpp
	${FIRMWARE_APPLICATION}/audio.cpp
	${FIRMWARE_APPLICATION}/baseband_api.cpp
	${FIRMWARE_APPLICATION}/capture_thread.cpp
	${FIRMWARE_APPLICATION}/clock_manager.cpp
	${FIRMWARE_APPLICATION}/core_control.cpp
	${FIRMWARE_APPLICATION}/database.cpp
	${FIRMWARE_APPLICATION}/de_bruijn.cpp
	#${FIRMWARE_APPLICATION}/emu_cc1101.cpp
	${FIRMWARE_APPLICATION}/rfm69.cpp
	${FIRMWARE_APPLICATION}/event_m0.cpp
	${FIRMWARE_APPLICATION}/file.cpp
	${FIRMWARE_APPLICATION}/freqman.cpp
	${FIRMWARE_APPLICATION}/io_file.cpp
	${FIRMWARE_APPLICATION}/io_wave.cpp
	${FIRMWARE_APPLICATION}/irq_controls.cpp
	${FIRMWARE_APPLICATION}/irq_lcd_frame.cpp
	${FIRMWARE_APPLICATION}/irq_rtc.cpp
	${FIRMWARE_APPLICATION}/log_file.cpp
	${FIRMWARE_APPLICATION}/portapack.cpp
	${FIRMWARE_APPLICATION}/qrcodegen.cpp
	${FIRMWARE_APPLICATION}/radio.cpp
	${FIRMWARE_APPLICATION}/receiver_model.cpp
	${FIRMWARE_APPLICATION}/recent_entries.cpp
	${FIRMWARE_APPLICATION}/replay_thread.cpp
	${FIRMWARE_APPLICATION}/rf_path.cpp
	${FIRMWARE_APPLICATION}/rtc_time.cpp
	${FIRMWARE_APPLICATION}/sd_card.cpp
	${FIRMWARE_APPLICATION}/serializer.cpp
	${FIRMWARE_APPLICATION}/spectrum_color_lut.cpp
	${FIRMWARE_APPLICATION}/string_format.cpp
	${FIRMWARE_APPLICATION}/temperature_logger.cpp
	${FIRMWARE_APPLICATION}/touch.cpp
	${FIRMWARE_APPLICATION}/tone_key.cpp
	${FIRMWARE_APPLICATION}/transmitter_model.cpp
	${FIRMWARE_APPLICATION}/tuning.cpp
	${FIRMWARE_APPLICATION}/hw/debounce.cpp
	${FIRMWARE_APPLICATION}/hw/encoder.cpp
	${FIRMWARE_APPLICATION}/hw/max2837.cpp
	${FIRMWARE_APPLICATION}/hw/max2839.cpp
	${FIRMWARE_APPLICATION}/hw/max5864.cpp
	${FIRMWARE_APPLICATION}/hw/rffc507x.cpp
	${FIRMWARE_APPLICATION}/hw/rffc507x_spi.cpp
	${FIRMWARE_APPLICATION}/hw/si5351.cpp
	${FIRMWARE_APPLICATION}/hw/spi_pp.cpp
	${FIRMWARE_APPLICATION}/hw/touch_adc.cpp
	#${FIRMWARE_APPLICATION}/ui_baseband_stats_view.cpp
	#${FIRMWARE_APPLICATION}/ui_navigation.cpp
	#${FIRMWARE_APPLICATION}/ui_playdead.cpp
	#${FIRMWARE_APPLICATION}/ui_record_view.cpp
	#${FIRMWARE_APPLICATION}/ui_sd_card_status_view.cpp
	#${FIRMWARE_APPLICATION}/ui/ui_alphanum.cpp
	#${FIRMWARE_APPLICATION}/ui/ui_audio.cpp
	#${FIRMWARE_APPLICATION}/ui/ui_channel.cpp
	${FIRMWARE_APPLICATION}/ui/ui_font_fixed_8x16.cpp
	#${FIRMWARE_APPLICATION}/ui/ui_geomap.cpp
    #${FIRMWARE_APPLICATION}/ui/ui_qrcode.cpp
	#${FIRMWARE_APPLICATION}/ui/ui_menu.cpp
	#${FIRMWARE_APPLICATION}/ui/ui_btngrid.cpp
	#${FIRMWARE_APPLICATION}/ui/ui_receiver.cpp
	#${FIRMWARE_APPLICATION}/ui/ui_rssi.cpp
	#${FIRMWARE_APPLICATION}/ui/ui_tv.cpp
	#${FIRMWARE_APPLICATION}/ui/ui_spectrum.cpp
	#${FIRMWARE_APPLICATION}/ui/ui_tabview.cpp
	#${FIRMWARE_APPLICATION}/ui/ui_textentry.cpp
	#${FIRMWARE_APPLICATION}/ui/ui_transmitter.cpp
	# ${FIRMWARE_APPLICATION}/apps/ui_about_simple.cpp
	# ${FIRMWARE_APPLICATION}/apps/ui_adsb_rx.cpp
	# ${FIRMWARE_APPLICATION}/apps/ui_adsb_tx.cpp
	# ${FIRMWARE_APPLICATION}/apps/ui_afsk_rx.cpp
	# ${FIRMWARE_APPLICATION}/apps/ui_aprs_rx.cpp
	# ${FIRMWARE_APPLICATION}/apps/ui_btle_rx.cpp
	# ${FIRMWARE_APPLICATION}/apps/ui_nrf_rx.cpp
	# ${FIRMWARE_APPLICATION}/apps/ui_aprs_tx.cpp
	# ${FIRMWARE_APPLICATION}/apps/ui_bht_tx.cpp
	# ${FIRMWARE_APPLICATION}/apps/ui_coasterp.cpp
	# ${FIRMWARE_APPLICATION}/apps/ui_debug.cpp
	# ${FIRMWARE_APPLICATION}/apps/ui_encoders.cpp
	# ${FIRMWARE_APPLICATION}/apps/ui_fileman.cpp
	# ${FIRMWARE_APPLICATION}/apps/ui_flash_utility.cpp
	# ${FIRMWARE_APPLICATION}/apps/ui_sd_over_usb.cpp
	# ${FIRMWARE_APPLICATION}/apps/ui_freqman.cpp
	# ${FIRMWARE_APPLICATION}/apps/ui_jammer.cpp
	# ${FIRMWARE_APPLICATION}/apps/ui_keyfob.cpp
	# ${FIRMWARE_APPLICATION}/apps/ui_lcr.cpp
	# ${FIRMWARE_APPLICATION}/apps/lge_app.cpp
	# ${FIRMWARE_APPLICATION}/apps/ui_looking_glass_app.cpp
	# ${FIRMWARE_APPLICATION}/apps/ui_mictx.cpp
	# ${FIRMWARE_APPLICATION}/apps/ui_modemsetup.cpp
	# ${FIRMWARE_APPLICATION}/apps/ui_morse.cpp
	# ${FIRMWARE_APPLICATION}/apps/ui_nuoptix.cpp
	# ${FIRMWARE_APPLICATION}/apps/ui_pocsag_tx.cpp
	# ${FIRMWARE_APPLICATION}/apps/ui_rds.cpp
	# ${FIRMWARE_APPLICATION}/apps/ui_remote.cpp
	# ${FIRMWARE_APPLICATION}/apps/ui_scanner.cpp
	# ${FIRMWARE_APPLICATION}/apps/ui_search.cpp
	# ${FIRMWARE_APPLICATION}/apps/ui_sd_wipe.cpp
	# ${FIRMWARE_APPLICATION}/apps/ui_settings.cpp
	# ${FIRMWARE_APPLICATION}/apps/ui_siggen.cpp
	# ${FIRMWARE_APPLICATION}/apps/ui_sonde.cpp
	# ${FIRMWARE_APPLICATION}/apps/ui_sstvtx.cpp
	# ${FIRMWARE_APPLICATION}/apps/ui_test.cpp
	# ${FIRMWARE_APPLICATION}/apps/ui_tone_search.cpp
	# ${FIRMWARE_APPLICATION}/apps/ui_touch_calibration.cpp
	# ${FIRMWARE_APPLICATION}/apps/ui_touchtunes.cpp
	# ${FIRMWARE_APPLICATION}/apps/ui_view_wav.cpp
	# ${FIRMWARE_APPLICATION}/apps/ui_whipcalc.cpp
	# ${FIRMWARE_APPLICATION}/apps/acars_app.cpp
	# ${FIRMWARE_APPLICATION}/apps/ais_app.cpp
	# ${FIRMWARE_APPLICATION}/apps/analog_audio_app.cpp
	# ${FIRMWARE_APPLICATION}/apps/analog_tv_app.cpp
	# ${FIRMWARE_APPLICATION}/apps/capture_app.cpp
	# ${FIRMWARE_APPLICATION}/apps/ert_app.cpp
	# ${FIRMWARE_APPLICATION}/apps/lge_app.cpp
	# ${FIRMWARE_APPLICATION}/apps/pocsag_app.cpp
	# ${FIRMWARE_APPLICATION}/apps/replay_app.cpp
	# ${FIRMWARE_APPLICATION}/apps/ui_playlist.cpp
	# ${FIRMWARE_APPLICATION}/apps/gps_sim_app.cpp
	# ${FIRMWARE_APPLICATION}/apps/soundboard_app.cpp
	# ${FIRMWARE_APPLICATION}/apps/ui_recon.cpp  
	# ${FIRMWARE_APPLICATION}/apps/ui_recon_settings.cpp
	# ${FIRMWARE_APPLICATION}/apps/ui_level.cpp  
	# ${FIRMWARE_APPLICATION}/apps/tpms_app.cpp
	# ${FIRMWARE_APPLICATION}/apps/tpms_app.cpp
	${FIRMWARE_APPLICATION}/protocols/aprs.cpp
	${FIRMWARE_APPLICATION}/protocols/ax25.cpp
	${FIRMWARE_APPLICATION}/protocols/bht.cpp
	${FIRMWARE_APPLICATION}/protocols/dcs.cpp
	${FIRMWARE_APPLICATION}/protocols/encoders.cpp
	${FIRMWARE_APPLICATION}/protocols/lcr.cpp
	${FIRMWARE_APPLICATION}/protocols/modems.cpp
	${FIRMWARE_APPLICATION}/protocols/rds.cpp
	# ${FIRMWARE_APPLICATION}/ui_handwrite.cpp
	# ${FIRMWARE_APPLICATION}/ui_loadmodule.cpp
	# ${FIRMWARE_APPLICATION}/ui_numbers.cpp
	# ${FIRMWARE_APPLICATION}/ui_replay_view.cpp
	# ${FIRMWARE_APPLICATION}/ui_script.cpp
	${FIRMWARE_APPLICATION}/ui_sd_card_debug.cpp
	${CPLD_20150901_DATA_CPP}
	${CPLD_20170522_DATA_CPP}
	${HACKRF_CPLD_DATA_CPP}
	lua_binding/lua_state.cpp
	lua_binding/ui/button.cpp
	ui_lua_view.cpp
)

# C sources to be compiled in ARM mode regardless of the global setting.
# NOTE: Mixing ARM and THUMB mode enables the -mthumb-interwork compiler
#       option that results in lower performance and larger code size.
set(ACSRC)

# C++ sources to be compiled in ARM mode regardless of the global setting.
# NOTE: Mixing ARM and THUMB mode enables the -mthumb-interwork compiler
#       option that results in lower performance and larger code size.
set(ACPPSRC)

# C sources to be compiled in THUMB mode regardless of the global setting.
# NOTE: Mixing ARM and THUMB mode enables the -mthumb-interwork compiler
#       option that results in lower performance and larger code size.
set(TCSRC)

# C sources to be compiled in THUMB mode regardless of the global setting.
# NOTE: Mixing ARM and THUMB mode enables the -mthumb-interwork compiler
#       option that results in lower performance and larger code size.
set(TCPPSRC)

# List ASM source files here
set(ASMSRC
	${PORTASM}
	${FIRMWARE_APPLICATION}/lz4.S
)

set(INCDIR ${CMAKE_CURRENT_BINARY_DIR} ${COMMON} ${PORTINC} ${KERNINC} ${TESTINC}
	${HALINC} ${PLATFORMINC} ${BOARDINC}
	${FATFSINC}
	${CHIBIOS}/os/various
	${FIRMWARE_APPLICATION}
	${FIRMWARE_APPLICATION}/ui
	${FIRMWARE_APPLICATION}/hw
	${FIRMWARE_APPLICATION}/apps
	${FIRMWARE_APPLICATION}/protocols
	${FIRMWARE_APPLICATION}/bitmaps
)

#
# Project, sources and paths
##############################################################################

##############################################################################
# Compiler settings
#

# TODO: Entertain using MCU=cortex-m0.small-multiply for LPC43xx M0 core.
# However, on GCC-ARM-Embedded 4.9 2015q2, it seems to produce non-functional
# binaries.
set(MCU cortex-m0)

# ARM-specific options here
set(AOPT)

# THUMB-specific options here
set(TOPT "-mthumb -DTHUMB")

# Define C warning options here
set(CWARN "-Wall -Wextra -Wstrict-prototypes")

# Define C++ warning options here
set(CPPWARN "-Wall -Wextra -Wno-psabi")

#
# Compiler settings
##############################################################################

##############################################################################
# Start of default section
#

# List all default C defines here, like -D_DEBUG=1
# TODO: Switch -DCRT0_INIT_DATA depending on load from RAM or SPIFI?
# NOTE: _RANDOM_TCC to kill a GCC 4.9.3 error with std::max argument types
set(DDEFS "-DLPC43XX -DLPC43XX_M0 -D__NEWLIB__ -DHACKRF_ONE -DLUA_OPTIMIZE_MEMORY=2 -DTOOLCHAIN_GCC -DTOOLCHAIN_GCC_ARM -D_RANDOM_TCC=0 -D'VERSION_STRING=\"${VERSION}\"'")

# List all default ASM defines here, like -D_DEBUG=1
set(DADEFS)

# List all default directories to look for include files here
set(DINCDIR)

# List the default directory to look for the libraries here
set(DLIBDIR)

# List all default libraries here
set(DLIBS)

#
# End of default section
##############################################################################

##############################################################################
# Start of user section
#

# List all user C define here, like -D_DEBUG=1
set(UDEFS)

# Define ASM defines here
set(UADEFS)

# List all user directories here
set(UINCDIR)

# List the user directory to look for the libraries here
set(ULIBDIR)

# List all user libraries here
set(ULIBS)

#
# End of user defines
##############################################################################

set(RULESPATH ${CHIBIOS}/os/ports/GCC/ARMCMx)
include(${RULESPATH}/rules.cmake)

##############################################################################

add_custom_command(
	OUTPUT ${CPLD_20150901_DATA_CPP}
	COMMAND ${EXTRACT_CPLD_DATA} ${CPLD_20150901_SVF_PATH} rev_20150901 >${CPLD_20150901_DATA_CPP}
	DEPENDS ${EXTRACT_CPLD_DATA} ${CPLD_20150901_SVF_PATH}
)

add_custom_command(
	OUTPUT ${CPLD_20170522_DATA_CPP}
	COMMAND ${EXTRACT_CPLD_DATA} ${CPLD_20170522_SVF_PATH} rev_20170522 >${CPLD_20170522_DATA_CPP}
	DEPENDS ${EXTRACT_CPLD_DATA} ${CPLD_20170522_SVF_PATH}
)

add_custom_command(
	OUTPUT ${HACKRF_CPLD_DATA_CPP}
	COMMAND ${HACKRF_CPLD_TOOL} --xsvf ${HACKRF_CPLD_XSVF_PATH} --portapack-data ${HACKRF_CPLD_DATA_CPP}
	DEPENDS ${HACKRF_CPLD_TOOL} ${HACKRF_CPLD_XSVF_PATH}
)

add_executable(${PROJECT_NAME}.elf ${CSRC} ${CPPSRC} ${ASMSRC})
set_target_properties(${PROJECT_NAME}.elf PROPERTIES LINK_DEPENDS ${LDSCRIPT})
add_definitions(${DEFS})
include_directories(. ${INCDIR})
link_directories(${LLIBDIR})
target_link_libraries(${PROJECT_NAME}.elf ${LIBS})
target_link_libraries(${PROJECT_NAME}.elf -Wl,-Map=${PROJECT_NAME}.map)

add_custom_command(
	OUTPUT ${PROJECT_NAME}.bin
	COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
	DEPENDS ${PROJECT_NAME}.elf
)

add_custom_target(
	${PROJECT_NAME}
	DEPENDS ${PROJECT_NAME}.bin
)
