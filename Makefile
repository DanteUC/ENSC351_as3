# Makefile for building embedded application.
# by Brian Fraser
# Edited by Divyam and Dante

# Edit this file to compile extra C files into their own programs.
TARGET= beatbox
SOURCES= main.c audioMixer.c matrix.c joystick.c utils.c buttons.c control.c beatGenerator.c 
PUBDIR = $(HOME)/cmpt433/public/myApps
OUTDIR = $(PUBDIR)
CROSS_TOOL = arm-linux-gnueabihf
CC_CPP = $(CROSS_TOOL)-g++
CC_C = $(CROSS_TOOL)-gcc

CFLAGS = -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror -Wshadow -Wall -pthread

# Asound Library
# - See the AudioGuide for steps to copy library from target to host.
LFLAGS = -L$(HOME)/cmpt433/public/asound_lib_BBB

# -pg for supporting gprof profiling.
#CFLAGS += -pg

all: beatbox wav

beatbox:
	$(CC_C) $(CFLAGS) $(SOURCES) -o $(OUTDIR)/$(TARGET) $(LFLAGS) -lpthread -lasound

clean:
	rm -f $(OUTDIR)/$(TARGET)

# Copy the sound files to public directory.
wav:
	mkdir -p $(PUBDIR)/beatbox-wav-files/
	cp -R beatbox-wav-files/* $(PUBDIR)/beatbox-wav-files/ 