#!/bin/bash

#Optionally predefined variables:
# DEBUG_MODE_ONLY_FILES

# Determines whether the specified name is in the list
# DEBUG_MODE_ONLY_FILES. If so 1 is returned, else 0.
#Args:
# 1) name of a file.
function isDebugModeOnlyFile {
    local RESULT=0
    for FILE in $DEBUG_MODE_ONLY_FILES
    do
        if [ $FILE == $1 ]
        then
            RESULT=1
            break
        fi
    done
    echo $RESULT
}

function removeDebugModeOnlyFiles {
    local LIST=$FILES
    FILES=
    for FILE in $LIST
    do
        if [ $(isDebugModeOnlyFile $FILE) -eq 0 ]
        then
            FILES="$FILES $FILE"
        fi
    done
}

# Looks for cpp-files in the current working directory
# and all of its subdirectories and appends the names of
# these files to global variable `FILES'. This function
# can be called without arguments to make the file names
# relative to the current working directory.
#Args:
# 1) name of the current directory
function findFilesToCompile {
    local LOCAL_FILES=$(ls *.cpp 2>/dev/null)
    for FILE in $LOCAL_FILES
    do
        FILES="$FILES $1$FILE"
    done

    local DIRS=$(ls -d */ 2>/dev/null)
    for DIR in $DIRS
    do
        cd $DIR
        findFilesToCompile "$1$DIR"
        cd ..
    done

    removeDebugModeOnlyFiles
}
