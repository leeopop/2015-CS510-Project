CC=or1knd-elf-gcc
CFLAGS=-msoft-div -msoft-float -msoft-mul -mno-ror -mno-cmov -mno-sext -nostdinc -nostdlib -static -nodefaultlibs -nostartfiles

###############################################################################
# Configuration
###############################################################################
# Target
TARGET ?= test_program

# Memory Layout
MEMBASE ?= 0x10000000
MEMSIZE ?= 16773216#12384
MEMNAME ?= sram

###############################################################################
# Files
###############################################################################
INCLUDE_DIRS ?= 
OBJ = boot.o
OBJS = $(patsubst %.c,%.o,$(wildcard *.c))
OBJ+= $(OBJS)

###############################################################################
## Makefile
###############################################################################

# Tools
OR1K_TCHAIN ?= or1knd-elf
CC          = $(OR1K_TCHAIN)-gcc $(CFLAGS)
AS          = $(OR1K_TCHAIN)-as
LD          = $(OR1K_TCHAIN)-ld
OBJDUMP     = $(OR1K_TCHAIN)-objdump
OBJCOPY     = $(OR1K_TCHAIN)-objcopy

# Options
CFLAGS 	   ?=
CFLAGS	   += -Ttext $(MEMBASE) -Os -g -Wall
CFLAGS     += -msoft-div -msoft-float -msoft-mul -mno-ror -mno-cmov -mno-sext
CFLAGS	   += -nostartfiles -nodefaultlibs -nostdlib -lgcc -L .
ASFLAGS     =
LDFLAGS     = 

LDSCRIPT= linker_script
CFLAGS += -mno-delay -D__OR1K_NODELAY__ -D__OR1K__
ASFLAGS+= -mno-delay -Wa,--defsym,__OR1K_NODELAY__=1
CFLAGS += -T$(LDSCRIPT)
SIMARGS += -n

CFLAGS += -I. $(INCLUDE_DIRS)

###############################################################################
# Rules
###############################################################################
all: $(TARGET).elf lst bin
	
clean:
	rm -f $(OBJ) *.map *.lst *.hex *.txt *.elf $(TARGET).bin

%.o : %.s
	$(CC) -c $(ASFLAGS) $< -o $@

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

$(TARGET).elf: $(OBJ) $(LDSCRIPT) Makefile
	$(CC) $(LDFLAGS) $(LIBS) $(OBJ) -o $@
	
lst:  $(TARGET).lst

%.lst: $(TARGET).elf
	$(OBJDUMP) -h -d -S $< > $@

bin: $(TARGET).bin

%.bin: %.elf
	$(OBJCOPY) -O binary $< $@
