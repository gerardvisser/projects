#!/bin/bash

#Optionally predefined variables:
# LIBS
# MODULES

# LIBS are libraries other than module libraries that should be linked, e.g.:
# LIBS="-lm"
# MODULES are the modules that the executable depends on, except for `testing',
# `core' and the module for which the tests are to be built.

TEST_DIR=$(pwd)
TEST_DIR_BASE=$(basename "$TEST_DIR")

if [[ $TEST_DIR_BASE == test && -f "../../testBuilder.sh" ]]
then

    source ../../filesToCompile.sh
    source ../../objBuilder.sh

    findFilesToCompile

    if [ -n "$FILES" ]
    then

        if [ ! -d "objs" ];
        then
            mkdir objs
        fi

        MODULE_NAME=$(basename ${TEST_DIR%/test*})

        if [ $MODULE_NAME != core ]
        then
            MODULES="$MODULE_NAME $MODULES"
        fi

        MODULES="$MODULES testing core"

        cd ..

        for MODULE in $MODULES
        do
            MODULE_LIBS="$MODULE_LIBS -llombok-$MODULE-d"
            INCLUDES="$INCLUDES -I../../$MODULE/include"
            cd ../$MODULE
            ./buildLib.sh
        done

        LIBS="$MODULE_LIBS $LIBS"

        cd ../$MODULE_NAME/test/

        createObjs "$FILES" "-DDEBUG_MODE" "objs"

        c++ -L../../../lib -orunTests.elf objs/*.o -pthread $LIBS

    fi

fi
