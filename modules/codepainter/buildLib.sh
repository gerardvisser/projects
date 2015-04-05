#!/bin/bash

source ../filesToCompile.sh

INCLUDES="-I../include -I../../core/include -I../../parsing/include"

LIB_BASE_NAME=lombok-codepainter

cd src
findFilesToCompile
cd ..

source ../libBuilder.sh
