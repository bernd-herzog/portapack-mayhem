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

project(baseband_shared-disarmed)

# Compiler options here.
set(USE_OPT "-O3 -g -falign-functions=16 -fno-math-errno --specs=nano.specs")

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
set(USE_FPU hard)

#
# Architecture or project specific options
##############################################################################

##############################################################################
# Project, sources and paths
#

# Imported source files and paths
include(${CHIBIOS_PORTAPACK}/boards/PORTAPACK_BASEBAND/board.cmake)
include(${CHIBIOS_PORTAPACK}/os/hal/platforms/LPC43xx_M4/platform.cmake)
include(${CHIBIOS}/os/hal/hal.cmake)
include(${CHIBIOS_PORTAPACK}/os/ports/GCC/ARMCMx/LPC43xx_M4/port.cmake)
include(${CHIBIOS}/os/kernel/kernel.cmake)

include(${CHIBIOS}/test/test.cmake)

# Define linker script file here
set(LDSCRIPT ${PORTLD}/LPC43xx_M4.ld)

# C sources that can be compiled in ARM or THUMB mode depending on the global
# setting.
set(CSRC
	${PORTSRC}
	${KERNSRC}
	${TESTSRC}
	${BOARDSRC}
)

set(FIRMWARE_BASEBAND ${FIRMWARE_SOURCE_DIR}/baseband)

# C++ sources that can be compiled in ARM or THUMB mode depending on the global
# setting.
set(CPPSRC
	${FIRMWARE_BASEBAND}/baseband.cpp
	${COMMON}/message_queue.cpp
	${COMMON}/event.cpp
	${FIRMWARE_BASEBAND}/event_m4.cpp
	${COMMON}/thread_wait.cpp
	${COMMON}/gpdma.cpp
	${FIRMWARE_BASEBAND}/baseband_dma.cpp
	${COMMON}/baseband_sgpio.cpp
	${COMMON}/portapack_shared_memory.cpp
	${COMMON}/buffer.cpp
	${FIRMWARE_BASEBAND}/baseband_thread.cpp
	${FIRMWARE_BASEBAND}/baseband_processor.cpp
	${FIRMWARE_BASEBAND}/baseband_stats_collector.cpp
	${FIRMWARE_BASEBAND}/dsp_decimate.cpp
	${FIRMWARE_BASEBAND}/dsp_demodulate.cpp
	${FIRMWARE_BASEBAND}/dsp_hilbert.cpp
	${FIRMWARE_BASEBAND}/dsp_modulate.cpp
	${FIRMWARE_BASEBAND}/dsp_goertzel.cpp
	${FIRMWARE_BASEBAND}/matched_filter.cpp
	${FIRMWARE_BASEBAND}/spectrum_collector.cpp
	${FIRMWARE_BASEBAND}/tv_collector.cpp
	${FIRMWARE_BASEBAND}/stream_input.cpp
	${FIRMWARE_BASEBAND}/stream_output.cpp
	${FIRMWARE_BASEBAND}/dsp_squelch.cpp
	${FIRMWARE_BASEBAND}/clock_recovery.cpp
	${FIRMWARE_BASEBAND}/packet_builder.cpp
	${COMMON}/dsp_fft.cpp
	${COMMON}/dsp_fir_taps.cpp
	${COMMON}/dsp_iir.cpp
	${COMMON}/dsp_sos.cpp
	${FIRMWARE_BASEBAND}/fxpt_atan2.cpp
	${FIRMWARE_BASEBAND}/rssi.cpp
	${FIRMWARE_BASEBAND}/rssi_dma.cpp
	${FIRMWARE_BASEBAND}/rssi_thread.cpp
	${FIRMWARE_BASEBAND}/audio_compressor.cpp
	${FIRMWARE_BASEBAND}/audio_output.cpp
	${FIRMWARE_BASEBAND}/audio_input.cpp
	${FIRMWARE_BASEBAND}/audio_dma.cpp
	${FIRMWARE_BASEBAND}/audio_stats_collector.cpp
	${COMMON}/utility.cpp
	${COMMON}/chibios_cpp.cpp
	${FIRMWARE_BASEBAND}/debug.cpp
	${COMMON}/gcc.cpp
	${FIRMWARE_BASEBAND}/tone_gen.cpp
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
set(ASMSRC ${PORTASM})

set(INCDIR ${COMMON} ${PORTINC} ${KERNINC} ${TESTINC}
	${HALINC} ${PLATFORMINC} ${BOARDINC}
	${CHIBIOS}/os/various
)

#
# Project, sources and paths
##############################################################################

##############################################################################
# Compiler settings
#

set(MCU cortex-m4)

# ARM-specific options here
set(AOPT)

# THUMB-specific options here
set(TOPT "-mthumb -DTHUMB")

# Define C warning options here
set(CWARN "-Wall -Wextra")

# Define C++ warning options here
set(CPPWARN "-Wall -Wextra")

#
# Compiler settings
##############################################################################

##############################################################################
# Start of default section
#

# List all default C defines here, like -D_DEBUG=1
# TODO: Switch -DCRT0_INIT_DATA depending on load from RAM or SPIFI?
# NOTE: _RANDOM_TCC to kill a GCC 4.9.3 error with std::max argument types
set(DDEFS "-DLPC43XX -DLPC43XX_M4 -D__NEWLIB__ -DHACKRF_ONE -DTOOLCHAIN_GCC -DTOOLCHAIN_GCC_ARM -D_RANDOM_TCC=0 -D'VERSION_STRING=\"${VERSION}\"'")

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

#######################################################################

add_library(${PROJECT_NAME} OBJECT ${CSRC} ${CPPSRC} ${ASMSRC})
include_directories(. ${INCDIR})

#######################################################################

set(BASEBAND_IMAGES)

macro(DeclareTargets chunk_tag name)
	project("baseband_disarm_${name}")

	include(${RULESPATH}/rules.cmake)
	add_executable(${PROJECT_NAME}.elf $<TARGET_OBJECTS:baseband_shared> ${MODE_CPPSRC} ${HALSRC} ${PLATFORMSRC})
	set_target_properties(${PROJECT_NAME}.elf PROPERTIES LINK_DEPENDS ${LDSCRIPT})
	add_definitions(${DEFS})
	target_compile_definitions(${PROJECT_NAME}.elf PUBLIC "-DBASEBAND_${name}")
	include_directories(. ${INCDIR} ${MODE_INCDIR} ${FIRMWARE_BASEBAND})
	link_directories(${LLIBDIR})
	target_link_libraries(${PROJECT_NAME}.elf ${LIBS})
	target_link_libraries(${PROJECT_NAME}.elf -Wl,-Map=${PROJECT_NAME}.map)

	add_custom_command(
		OUTPUT ${PROJECT_NAME}.bin ${PROJECT_NAME}.img
		COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
		COMMAND ${LZ4} -f -9 ${PROJECT_NAME}.bin ${PROJECT_NAME}.lz4
		COMMAND ${MAKE_IMAGE_CHUNK} ${PROJECT_NAME}.lz4 ${chunk_tag} ${PROJECT_NAME}.img
		DEPENDS ${PROJECT_NAME}.elf ${MAKE_IMAGE_CHUNK}
		VERBATIM
	)

	set(BASEBAND_IMAGES ${BASEBAND_IMAGES} ${PROJECT_NAME}.img)
endmacro()

### Flash Utility

include(${CHIBIOS_PORTAPACK}/os/various/fatfs_bindings/fatfs.cmake)
set(MODE_INCDIR ${FATFSINC})
set(MODE_CPPSRC
	${FIRMWARE_BASEBAND}/proc_flash_utility.cpp
	${FIRMWARE_BASEBAND}/w25q80bv.cpp
	${FATFSSRC}
)
DeclareTargets(PFUT flash_utility)

### SD over USB

set(MODE_INCDIR
	${HACKRF_PATH}/firmware
	${HACKRF_PATH}/firmware/common
	${HACKRF_PATH}/firmware/libopencm3/include
)
set(MODE_CPPSRC
	${FIRMWARE_BASEBAND}/sd_over_usb/proc_sd_over_usb.cpp
	
	${FIRMWARE_BASEBAND}/sd_over_usb/scsi.c
	${FIRMWARE_BASEBAND}/sd_over_usb/diskio.c
	${FIRMWARE_BASEBAND}/sd_over_usb/sd_over_usb.c
	${FIRMWARE_BASEBAND}/sd_over_usb/usb_descriptor.c
	${FIRMWARE_BASEBAND}/sd_over_usb/hackrf_core.c

	${HACKRF_PATH}/firmware/common/usb.c
	${HACKRF_PATH}/firmware/common/usb_queue.c
	${HACKRF_PATH}/firmware/hackrf_usb/usb_device.c
	${HACKRF_PATH}/firmware/hackrf_usb/usb_endpoint.c
	${HACKRF_PATH}/firmware/common/usb_request.c
	${HACKRF_PATH}/firmware/common/usb_standard_request.c
	${HACKRF_PATH}/firmware/common/platform_detect.c
	${HACKRF_PATH}/firmware/common/gpio_lpc.c
	${HACKRF_PATH}/firmware/common/firmware_info.c
	${HACKRF_PATH}/firmware/common/si5351c.c
	${HACKRF_PATH}/firmware/common/i2c_bus.c
	${HACKRF_PATH}/firmware/common/mixer.c
	${HACKRF_PATH}/firmware/common/clkin.c
	${HACKRF_PATH}/firmware/common/spi_bus.c
	${HACKRF_PATH}/firmware/common/sgpio.c
	${HACKRF_PATH}/firmware/common/rf_path.c
	${HACKRF_PATH}/firmware/common/i2c_lpc.c
	${HACKRF_PATH}/firmware/common/spi_ssp.c
	${HACKRF_PATH}/firmware/common/rffc5071_spi.c
	${HACKRF_PATH}/firmware/common/rffc5071.c
	${HACKRF_PATH}/firmware/common/clkin.c
	${HACKRF_PATH}/firmware/common/gpdma.c

	${HACKRF_PATH}/firmware/libopencm3/lib/cm3/nvic.c
	${HACKRF_PATH}/firmware/libopencm3/lib/cm3/sync.c
	${HACKRF_PATH}/firmware/libopencm3/lib/lpc43xx/scu.c
	${HACKRF_PATH}/firmware/libopencm3/lib/lpc43xx/timer.c
	${HACKRF_PATH}/firmware/libopencm3/lib/lpc43xx/i2c.c
)
DeclareTargets(PUSB sd_over_usb)

### Lua general purpose rx

set(MODE_CPPSRC
	proc_lua_rx.cpp
)
DeclareTargets(PLUR lua_rx)

### HackRF "factory" firmware

add_custom_command(
	OUTPUT hackrf.img
	COMMAND ${LZ4} -f -9 ${HACKRF_FIRMWARE_BIN_IMAGE} hackrf.lz4
	COMMAND ${MAKE_IMAGE_CHUNK} hackrf.lz4 HRF1 hackrf.img
	DEPENDS ${HACKRF_FIRMWARE_BIN_FILENAME} ${MAKE_IMAGE_CHUNK}
	VERBATIM
)

set(BASEBAND_IMAGES ${BASEBAND_IMAGES} hackrf.img)

### Terminator image

add_custom_command(
	OUTPUT terminator.img
	COMMAND ${MAKE_IMAGE_CHUNK} terminator.img
	DEPENDS ${MAKE_IMAGE_CHUNK}
	VERBATIM
)

set(BASEBAND_IMAGES ${BASEBAND_IMAGES} terminator.img)

#######################################################################

project(baseband-disarmed)
	
add_custom_command(
	OUTPUT ${PROJECT_NAME}.img
	COMMAND cat ${BASEBAND_IMAGES} > ${PROJECT_NAME}.img
	DEPENDS ${BASEBAND_IMAGES}
	DEPENDS hackrf.img terminator.img
	VERBATIM
)

add_custom_target(
	${PROJECT_NAME}
	DEPENDS ${PROJECT_NAME}.img
)
