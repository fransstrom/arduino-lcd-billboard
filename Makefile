# CC := avr-gcc
# LD := avr-ld
# OBJCOPY := avr-objcopy
# OBJISP := avrdude
# PORT := /dev/ttyACM0
#
# MCU := atmega328p
#
# CFLAGS := -Wall -Wextra  -Wundef -pedantic \
# 		-Os -std=gnu99 -DF_CPU=16000000UL -mmcu=${MCU}
# LDFLAGS := -mmcu=$(MCU)
#
# BIN := program.hex
# SOURCES := main.c lcd.c
# OBJS := $(SOURCES:.c=.o)
#
# all: $(BIN)
#
# %.o:%.c
# 	$(COMPILE.c) -MD -o $@ $<
#
# %.elf: $(OBJS)
# 	$(CC) -Wl,-Map=$(@:.elf=.map) $(LDFLAGS) -o $@ $^
#
# %.hex: %.elf
# 	$(OBJCOPY) -O ihex -R .fuse -R .lock -R .user_signatures -R .comment $< $@
#
# isp: ${BIN}
# 	$(OBJISP) -F -V -c arduino -p ${MCU} -P ${PORT} -U flash:w:$<
#
# clean:
# 	@rm -f $(BIN) $(OBJS) *.map *.P *.d
#
# CC := avr-gcc
# LD := avr-ld
# OBJCOPY := avr-objcopy
# OBJISP := avrdude
# PORT := /dev/ttyACM0
# MCU := atmega328p
# CFLAGS := -Wall -Wextra -Wundef -pedantic \
# 		-Os -std=gnu99 -DF_CPU=16000000UL -mmcu=${MCU}
# LDFLAGS := -mmcu=$(MCU)
# SRC := src
# BUILD := build
# ELF := $(BUILD)/program.elf
# BIN := $(BUILD)/program.hex
# SOURCES := $(SRC)/main.c $(SRC)/lcd.c
# OBJS := $(addprefix $(BUILD)/, $(notdir $(SOURCES:.c=.o)))
#
# all: $(BIN)
#
# $(BUILD):
# 	mkdir -p $(BUILD)
#
# $(BUILD)/%.o: $(SRC)/%.c | $(BUILD)
# 	$(CC) $(CFLAGS) -c -MD -MF $(BUILD)/$*.d -o $@ $
#
# $(ELF): $(OBJS)
# 	$(CC) -Wl,-Map=$(BUILD)/program.map $(LDFLAGS) -o $@ $^
#
# $(BIN): $(ELF)
# 	$(OBJCOPY) -O ihex -R .fuse -R .lock -R .user_signatures -R .comment $< $@
#
# isp: $(BIN)
# 	$(OBJISP) -F -V -c arduino -p ${MCU} -P ${PORT} -U flash:w:$
#
# clean:
# 	@rm -rf $(BUILD)


CC := avr-gcc
LD := avr-ld
OBJCOPY := avr-objcopy
OBJISP := avrdude
PORT := /dev/ttyACM0
MCU := atmega328p
CFLAGS := -Wall -Wextra -Wundef -pedantic \
		-Os -std=gnu99 -DF_CPU=16000000UL -mmcu=${MCU}
LDFLAGS := -mmcu=$(MCU)
SRC := src
BUILD := build
ELF := $(BUILD)/program.elf
BIN := $(BUILD)/program.hex
SOURCES := $(SRC)/main.c $(SRC)/lcd.c $(SRC)/billboard.c 
OBJS := $(addprefix $(BUILD)/, $(notdir $(SOURCES:.c=.o)))

all: $(BIN)

$(BUILD):
	mkdir -p $(BUILD)

# $(BUILD)/%.o: $(SRC)/%.c | $(BUILD)
# 	$(CC) $(CFLAGS) -c -MD -MF $(@:.o=.d) -o $@ $
$(BUILD)/%.o: $(SRC)/%.c | $(BUILD)
	$(CC) $(CFLAGS) -c -MD -MF $(@:.o=.d) -o $@ $(SRC)/$*.c
$(ELF): $(OBJS)
	$(CC) -Wl,-Map=$(BUILD)/program.map $(LDFLAGS) -o $@ $^

$(BIN): $(ELF)
	$(OBJCOPY) -O ihex -R .fuse -R .lock -R .user_signatures -R .comment $< $@

isp: $(BIN)
	$(OBJISP) -F -V -c arduino -p ${MCU} -P ${PORT} -U flash:w:$(BIN)
clean:
	@rm -rf $(BUILD)
