#!/bin/bash

EXE_FILE=debug.elf

MODULES="core"

INCLUDES="`pkg-config --cflags gtk+-3.0`"

LIBS="`pkg-config --libs gtk+-3.0`"

source ../buildApp.sh
