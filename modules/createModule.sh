#!/bin/bash

#First argument to this script is the name of the module to be created;
#the remaining arguments are the modules it depends on, except for 'core'.

if [ $# -lt 1 ]
then
    echo "No module name specified."
    exit 1
elif [ -e $1 ]
then
    echo "A file with the specified module name already exists."
    exit 1
elif [ $# -gt 1 ]
then
    MODULES="$@"
    MODULES=${MODULES#$1 }

    for MODULE in $MODULES
    do
        EXTRA_INCLUDES="$EXTRA_INCLUDES -I../../$MODULE/include"
    done
fi

GPL_MESSAGE=$'/*\n   Author:  Gerard Visser\n   e-mail:  visser.gerard(at)gmail.com\n\n   Copyright (C) 2013 Gerard Visser.\n\n   This program is free software: you can redistribute it and/or modify\n   it under the terms of the GNU General Public License as published by\n   the Free Software Foundation; either version 3 of the License, or\n   (at your option) any later version.\n\n   This program is distributed in the hope that it will be useful,\n   but WITHOUT ANY WARRANTY; without even the implied warranty of\n   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n   GNU General Public License for more details.\n\n   You should have received a copy of the GNU General Public License\n   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */\n\n'

mkdir $1

cd $1
mkdir include
mkdir src
mkdir test
echo $'#!/bin/bash\n\nsource ../filesToCompile.sh\n\nINCLUDES=\"-I../include -I../../core/include'"$EXTRA_INCLUDES"$'\"\n\nLIB_BASE_NAME=lombok-'$1$'\n\ncd src\nfindFilesToCompile\ncd ..\n\nsource ../libBuilder.sh' > buildLib.sh
chmod 775 buildLib.sh

cd include
mkdir lombok

cd ../src
mkdir lombok

cd ../test

echo "$GPL_MESSAGE"$'#ifndef ___ALL_TESTS_INCLUDED\n#define ___ALL_TESTS_INCLUDED\n\n#endif' > allTests.h

echo "$GPL_MESSAGE"$'#include <lombok/testing/testUtils.h>\n#include \"allTests.h\"\n\nvoid testRunner_runTests (void) {\n  ___BTPUSH;\n\n\n  ___BTPOP;\n}' > testRunner.cpp

echo $'#!/bin/bash\n' > buildTestRunner.sh
if [ -n "$MODULES" ]
then
    echo $'MODULES=\"'"$MODULES"$'\"\n' >> buildTestRunner.sh
fi
echo $'source ../../testBuilder.sh' >> buildTestRunner.sh
chmod 775 buildTestRunner.sh
