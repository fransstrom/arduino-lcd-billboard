CC := avr-gcc
LD := avr-ld
OBJCOPY := avr-objcopy
OBJISP := avrdude
# PORT := /dev/ttyACM0
PORT := /dev/ttyACM1
MCU := atmega328p
CFLAGS := -Wall -Wextra -Wundef -pedantic \
		-Os -std=gnu99 -DF_CPU=16000000UL -mmcu=${MCU} -DBAUD=9600
LDFLAGS := -mmcu=$(MCU)
SRC := src
BUILD := build
ELF := $(BUILD)/program.elf
BIN := $(BUILD)/program.hex
SOURCES := $(SRC)/main.c $(SRC)/lcd.c $(SRC)/billboard.c $(SRC)/millis.c $(SRC)/uart.c $(SRC)/company.c $(SRC)/ad.c $(SRC)/millis_helper.c
OBJS := $(addprefix $(BUILD)/, $(notdir $(SOURCES:.c=.o)))
BAUD := 9600
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

monitor:
	screen $(PORT) $(BAUD)
