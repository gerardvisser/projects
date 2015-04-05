#!/bin/bash

source ../filesToCompile.sh

INCLUDES="-I../include -I../../core/include"

LIB_BASE_NAME=lombok-parsing

TEST_MODE_FILES="\
 lombok/parsing/tokenisers/tokeniser.cpp\
"

cd src
findFilesToCompile
cd ..

source ../libBuilder.sh
