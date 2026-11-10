CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

ROOTDIR := $(CURDIR)
SOURCEDIR = ${ROOTDIR}/src
INCLUDEDIR = ${ROOTDIR}/include
SCRIPTSDIR = ${ROOTDIR}/script

CFLAGS  ?=  -W -Wall -Wextra -Werror -Wundef -Wshadow -Wdouble-promotion \
	-Wformat-truncation -fno-common -Wconversion \
	-g3 -Og -ffunction-sections -fdata-sections -I. -I${INCLUDEDIR} \
	-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 $(EXTRA_CFLAGS)

LDFLAGS ?= -T${SCRIPTSDIR}/f446.ld -nostartfiles -nostdlib --specs nano.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=$@.map

SRCS = $(wildcard ${SOURCEDIR}/*.c)
OBJS = $(SRCS:.c=.o)

all: firmware.bin
	clean_o
firmware.elf: $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) $(LDFLAGS) -o $@

firmware.bin: firmware.elf
	$(OBJCOPY) -O binary $< $@

flash: firmware.bin
	st-flash --reset write $< 0x8000000

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean_o:
	rm -r $(OBJS)
clean:
	rm -rf firmware.*
	clean_o

.PHONY: all clean flash
