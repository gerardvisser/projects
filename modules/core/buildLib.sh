#!/bin/bash

source ../filesToCompile.sh

DEBUG_SETTINGS="-DIO_BUF_SIZE=0x200"

INCLUDES="-I../include"

LIB_BASE_NAME=lombok-core

DEBUG_MODE_ONLY_FILES="\
 lombok/base/dbgAllocInfo.cpp\
 lombok/base/dbgMemStats.cpp\
 lombok/base/dbgRefInfo.cpp\
 lombok/base/dbgRefInfoHashList.cpp\
 lombok/base/dbgRefInfoList.cpp\
 lombok/base/mem.cpp\
"

cd src
findFilesToCompile
cd ..

source ../libBuilder.sh
