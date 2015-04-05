#!/bin/bash

INCLUDES="-I../include -I../../core/include"

LIB_BASE_NAME=lombok-testing

FILES="\
 lombok/testing/runTests.cpp\
 lombok/testing/testUtils.cpp\
"

source ../libBuilder.sh
