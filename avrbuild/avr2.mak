OURPATH		= avrbuild
SCONS		= scons
AVRDUDE		= avrdude
PROG		= usbasp
PORT		= /dev/ttyUSB0

.PHONY: all
all: build

.PHONY: build
build:
	$(SCONS) -f $(OURPATH)/avr.scons NAME=$(NAME) MCU=$(CMCU) FREQ=$(FREQ) ADDCFLAGS="$(CFLAGS)" ADDCXXFLAGS="$(CXXFLAGS)" ADDLDFLAGS="$(LDFLAGS)"

.PHONY: poke
poke: build
	$(AVRDUDE) -V -i 5 $(DUDEOPT) -p $(DMCU) -c $(PROG) -P $(PORT) -e -U lfuse:w:0x$(FUSEL):m -U hfuse:w:0x$(FUSEH):m -U flash:w:build/$(NAME).hex

.PHONY: clean
clean:
	$(SCONS) -c -f $(OURPATH)/avr.scons NAME=$(NAME) MCU=$(CMCU) FREQ=$(FREQ) ADDCFLAGS="$(CFLAGS)" ADDCXXFLAGS="$(CXXFLAGS)" ADDLDFLAGS="$(LDFLAGS)"

