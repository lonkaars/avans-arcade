RM = rm -f

TARGET = main

# platform is ds (desktop) or stm (stm32)
PLATFORM = ds
# if your editor uses compile_commands.json for autocomplete, you should run `make compile_commands.json` again

HOST=$(strip $(shell uname -o))

ifeq ($(PLATFORM),stm)
STM = true
include stm32.mk
endif
ifeq ($(PLATFORM),ds)
DESKTOP = true
include ds.mk
endif

SHARED_FLAGS += -g
SHARED_FLAGS += -Wall
SHARED_FLAGS += -Wextra
SHARED_FLAGS += -I.
# SHARED_FLAGS += -O1
LFLAGS += -lm
# LFLAGS += -lc
CFLAGS += $(if $(STM), -DHH_TARGET_STM32, )
CFLAGS += $(if $(DESKTOP), -DHH_TARGET_DESKTOP, )

LOCAL_SRCS += main.c \
							ppu/internals.c \
							ppu/ppu.c \
							demo.c \
							engine/engine.c \
							engine/sprite_controller.c \
							engine/draw_screen.c \
							engine/camera.c \
							engine/maths.c \
							engine/entity.c

CFLAGS += $(SHARED_FLAGS)
LFLAGS += $(SHARED_FLAGS)
AFLAGS += $(SHARED_FLAGS)

SRCS += $(LOCAL_SRCS)
SRCS += $(if $(STM), $(STM_SRCS), )
SRCS += $(if $(DESKTOP), $(DESKTOP_SRCS), )

ifeq ($(PLATFORM),stm)
OBJS := $(patsubst %.c,%-stm.o, $(patsubst %.s,%-stm.o, $(SRCS)))
all: $(TARGET).bin
endif
ifeq ($(PLATFORM),ds)
OBJS := $(patsubst %.c,%-ds.o, $(SRCS))
all: $(TARGET)
endif

%-stm.o: %.s
	$(CC) -c $(AFLAGS) $< -o $@

lib/%-stm.o: lib/%.c
	$(CC) -c $(CFLAGS) -w $< -o $@

%-stm.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

$(TARGET).elf: $(OBJS)
	$(LD) $^ $(LFLAGS) -o $@

$(TARGET).bin: $(TARGET).elf
	$(OC) -O binary $< $@
	$(OS) $<

PHONY += flash
flash: $(TARGET).bin
	st-flash --reset write $(TARGET).bin 0x08000000

%-ds.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

$(TARGET): $(OBJS)
	$(LD) $^ $(LFLAGS) -o $@

compile_commands.json: makefile stm32.mk ds.mk
	compiledb make -Bn
	../scripts/compiledb-full-path-mingw.sh compile_commands.json

PHONY += format
format:
	clang-format -i $(LOCAL_SRCS) $(DESKTOP_SRCS)
	clang-tidy --fix-errors $(LOCAL_SRCS) $(DESKTOP_SRCS)

PHONY += clean
clean:
	$(RM) $(TARGET).bin $(TARGET).elf $(OBJS)

.PHONY: $(PHONY)
