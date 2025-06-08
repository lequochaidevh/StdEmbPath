#!/bin/bash

BUILDSRC=$TESTDIR
PRJNAME="exam2"
CFLAGS="-lm -g"
COMPILER="gcc"
DISTINATION="-o $BUILDDIR"
SOURCE="$TESTDIR/$PRJNAME.c"
LIBSRC="$TESTDIR/../matrixCalcBase.c"
CMD=($COMPILER)
InsertWithoutEmpty() {
    if [ -n "$1" ]; then
        CMD="$CMD $1"
    fi
}

BuildBIN() {
    cd $BUILDSRC
    #set -x
    InsertWithoutEmpty "$SOURCE"
    InsertWithoutEmpty "$LIBSRC"
    InsertWithoutEmpty "$DISTINATION$PRJNAME"
    InsertWithoutEmpty "$CFLAGS"
    InsertWithoutEmpty "$CPPFLAGS"
    echo $CMD
    $CMD
    cd -
}

RunBIN() {
	cd $BUILDDIR
	echo RUN...
	./$PRJNAME
	cd -
}

clear

echo "BUILDSRC: $BUILDSRC"
echo "BUILDDIR: $BUILDDIR"
echo "TESTDIR: $TESTDIR"
echo "RUNDIR: $RUNDIR"

BuildBIN

RunBIN
