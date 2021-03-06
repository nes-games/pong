ROMNAME = pong.nes
BOARD = nrom-128

SRCDIR = src
OBJDIR = obj
EMUEXE = nes
EMUOPT =
MAP = $(ROMNAME:.nes=.m)
LIB = $(SRCDIR)/familib/runtime.lib
CC = cc65
CA = ca65
LD = ld65
CFLAGS = -t nes -Oi --add-source

rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))
CRT0 = $(strip $(call rwildcard, $(SRCDIR), */$(BOARD).inc))
CFG = $(CRT0:.inc=.cfg)
CSOURCES = $(call rwildcard, $(SRCDIR), *.c)
SSOURCES = $(call rwildcard, $(SRCDIR), *.s)
DEPS := $(CSOURCES:.c=.d)
DEPS := $(DEPS:$(SRCDIR)/%=$(OBJDIR)/%)
OBJS := $(CRT0:.inc=.o) $(SSOURCES:.s=.o) $(CSOURCES:.c=.o)
OBJS := $(OBJS:$(SRCDIR)/%=$(OBJDIR)/%)

ifeq ($(shell echo),)
	MKDIR = mkdir -p $1
else
	MKDIR = mkdir $(subst /,\,$1)
endif

ifneq ($(MAKECMDGOALS),clean)
-include $(DEPS)
endif

$(ROMNAME): $(CFG) $(CRT0) $(SSOURCES) $(OBJS) 
	$(LD) -C $(CFG) -m $(MAP) -o $@ $(OBJS) $(LIB)

all: clean play

play: $(ROMNAME)
	$(EMUEXE) $(EMUOPT) $(ROMNAME)

.SECONDEXPANSION:

$(OBJDIR)/%.o: $(OBJDIR)/%.s | $$(@D)/.
	$(CA) $< -o $@
	$(RM) $<

$(OBJDIR)/%.o: $(SRCDIR)/%.s | $$(@D)/.
	$(CA) $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.inc | $$(@D)/.
	$(CA) $< -o $@

$(OBJDIR)/%.s: $(SRCDIR)/%.c | $$(@D)/.
	$(CC) $(CFLAGS) -o $@ --create-dep $(@:.s=.d) $<

.PRECIOUS: $(OBJDIR)/. $(OBJDIR)%/.

$(OBJDIR)/.:
	$(call MKDIR,$@)

$(OBJDIR)%/.:
	$(call MKDIR,$@)

.PHONY: clean

clean:
	$(RM) $(OBJS) $(DEPS) $(MAP) $(ROMNAME)
