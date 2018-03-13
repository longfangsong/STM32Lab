import os

# toolchains options
ARCH = 'arm'
CPU = 'cortex-m3'
CROSS_TOOL = 'gcc'

if os.getenv('RTT_CC'):
    CROSS_TOOL = os.getenv('RTT_CC')

# device options
# STM32_TYPE =
# 'STM32F10X_LD','STM32F10X_LD_VL',
# 'STM32F10X_MD','STM32F10X_MD_VL',
# 'STM32F10X_HD','STM32F10X_HD_VL',
# 'STM32F10X_XL','STM32F10X_CL'
STM32_TYPE = 'STM32F10X_HD'

# lcd panel options
# 'FMT0371','ILI932X', 'SSD1289'
RT_USING_LCD_TYPE = 'ILI9341'

# cross_tool provides the cross compiler
# EXEC_PATH is the compiler execute path, for example, CodeSourcery, Keil MDK, IAR

if CROSS_TOOL == 'gcc':
    PLATFORM = 'gcc'
    EXEC_PATH = '/Users/longfangsong/program_library/gcc-arm-none-eabi/bin/'

if os.getenv('RTT_EXEC_PATH'):
    EXEC_PATH = os.getenv('RTT_EXEC_PATH')

BUILD = 'debug'

if PLATFORM == 'gcc':
    # toolchains
    PREFIX = 'arm-none-eabi-'
    CC = PREFIX + 'gcc'
    AS = PREFIX + 'gcc'
    AR = PREFIX + 'ar'
    LINK = PREFIX + 'gcc'
    TARGET_EXT = 'axf'
    SIZE = PREFIX + 'size'
    OBJDUMP = PREFIX + 'objdump'
    OBJCPY = PREFIX + 'objcopy'

    DEVICE = ' -mcpu=cortex-m3 -mthumb -ffunction-sections -fdata-sections'
    CFLAGS = DEVICE
    AFLAGS = ' -c' + DEVICE + ' -x assembler-with-cpp'
    LFLAGS = DEVICE + ' -Wl,--gc-sections,-Map=rtthread-stm32.map,-cref,-u,Reset_Handler -T stm32_rom.ld'

    CPATH = ''
    LPATH = ''

    if BUILD == 'debug':
        CFLAGS += ' -O0 -gdwarf-2'
        AFLAGS += ' -gdwarf-2'
    else:
        CFLAGS += ' -O2'

    POST_ACTION = OBJCPY + ' -O binary $TARGET rtthread.bin\n' + SIZE + ' $TARGET \n'
