#!/bin/bash

# Note:
# Do not run this script directly from the command line:
# it is meant to be included by other scripts.

#Predefined variables:
# FILES
# INCLUDES
# LIB_BASE_NAME

#Optionally predefined variables:
# DEBUG_MODE_ONLY_FILES
# DEBUG_SETTINGS
# TEST_MODE_FILES

source ../objBuilder.sh

#Args:
# 1) base name of the archive.
function createLibrary {
    cd ../objs
    ar rcs lib$1.a *.o
    mv *.a ../../../lib
    cd ../src
}

#Args:
# 1) library name suffix.
# 2) compiler options.
# 3) object file directory.
function createTestModeLibraryIfNecessary {
    if [ -n "$TEST_MODE_FILES" ]
    then
        deleteCorrespondingObjFiles "$TEST_MODE_FILES" "$3"
        createObjs "$TEST_MODE_FILES" "-DTEST_MODE $2" "$3"
        createLibrary $LIB_BASE_NAME$1
    fi
}

#Args:
# 1) files for which to delete the corresponding object file.
# 2) object file directory.
function deleteCorrespondingObjFiles {
    for SRC_FILE in $1
    do
        OBJ_FILE=$(getObjFileName "$SRC_FILE" "$2")
        rm -f $OBJ_FILE
    done
}

if [ -n "$FILES" ]
then

    if [ ! -d "../../lib" ]
    then
        mkdir ../../lib
    fi

    if [ ! -f "../../lib/lib$LIB_BASE_NAME.a" ]
    then

        if [ ! -d "objs" ]
        then
            mkdir objs
        else
            rm -f objs/*
        fi

        cd src

        OBJS_DIR="../objs"

# optimised mode:
        COMPILER_OPTS="-O2"

        createObjs "$FILES" "$COMPILER_OPTS" "$OBJS_DIR"
        createLibrary $LIB_BASE_NAME
        createTestModeLibraryIfNecessary -t "$COMPILER_OPTS" "$OBJS_DIR"
        rm -f $OBJS_DIR/*


# debug mode:
        COMPILER_OPTS="-DDEBUG_MODE $DEBUG_SETTINGS"

        createObjs "$FILES $DEBUG_MODE_ONLY_FILES" "$COMPILER_OPTS" "$OBJS_DIR"
        createLibrary $LIB_BASE_NAME-d
        createTestModeLibraryIfNecessary -t-d "$COMPILER_OPTS" "$OBJS_DIR"


        cd ..

    fi

fi
