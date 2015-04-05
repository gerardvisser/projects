#!/bin/bash

#Predefined variables:
# INCLUDES

#Args:
# 1) files to compile.
# 2) compiler options.
# 3) object file directory.
function createObjs {
    for SRC_FILE in $1
    do
        OBJ_FILE=$(getObjFileName "$SRC_FILE" "$3")
        if [ "$SRC_FILE" -nt "$OBJ_FILE" ]
        then
            c++ -pthread $INCLUDES $2 -c -o$OBJ_FILE $SRC_FILE
        fi
    done
}

#Args:
# 1) name of the source file.
# 2) object file directory.
#Returns:
# name of the object file associated with the specified source file.
function getObjFileName {
    NAME=$(basename "$1")
    BASE="${NAME%.*}"
    echo "$2/$BASE.o"
}
