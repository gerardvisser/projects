#!/bin/bash

# This script is intended to be included in other scripts in one of the subdirectories
# of the directory this file is in.

#Predefined variables:
# EXE_FILE: the name of the executable file this script should produce.
# MODULES: the modules to be included in the executable, e.g. MODULES="core".

#Optionally predefined variables:
# LIBS: libraries other than module libraries, e.g. LIBS="-lm"

source ../../modules/filesToCompile.sh
source ../../modules/objBuilder.sh

APP_DIR=$(pwd)

if [[ $# -ge 1 && $1 == o ]]
then
    OBJS_DIR=objs
    COMPILER_OPTS="-O2"
    LINKER_OPTS="-s"
else
    LIB_SUFFIX=-d
    OBJS_DIR=dbgobjs
    COMPILER_OPTS="-DDEBUG_MODE"
fi

cd ../../modules

for MODULE in $MODULES
do
    MODULE_DIR="${MODULE%-t}"
    MODULE_LIBS="$MODULE_LIBS -llombok-$MODULE$LIB_SUFFIX"
    INCLUDES="$INCLUDES -I../../modules/$MODULE_DIR/include"
    cd $MODULE_DIR
    ./buildLib.sh
    cd ..
done

LIBS="$MODULE_LIBS $LIBS"

cd "$APP_DIR"

findFilesToCompile

if [ -n "$FILES" ]
then
    if [ ! -d $OBJS_DIR ]
    then
        mkdir $OBJS_DIR
    fi
    createObjs "$FILES" "$COMPILER_OPTS" "$OBJS_DIR"
    c++ -L../../lib $LINKER_OPTS -o$EXE_FILE $OBJS_DIR/*.o -pthread $LIBS
fi
