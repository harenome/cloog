#!/bin/sh
#
#   /**-------------------------------------------------------------------**
#    **                              CLooG                                **
#    **-------------------------------------------------------------------**
#    **                           checker.sh                              **
#    **-------------------------------------------------------------------**
#    **                 First version: November 16th 2011                 **
#    **-------------------------------------------------------------------**/
#

#/*****************************************************************************
# *               CLooG : the Chunky Loop Generator (experimental)            *
# *****************************************************************************
# *                                                                           *
# * Copyright (C) 2003 Cedric Bastoul                                         *
# *                                                                           *
# * This library is free software; you can redistribute it and/or             *
# * modify it under the terms of the GNU Lesser General Public                *
# * License as published by the Free Software Foundation; either              *
# * version 2.1 of the License, or (at your option) any later version.        *
# *                                                                           *
# * This library is distributed in the hope that it will be useful,           *
# * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
# * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU         *
# * Lesser General Public License for more details.                           *
# *                                                                           *
# * You should have received a copy of the GNU Lesser General Public          *
# * License along with this library; if not, write to the Free Software       *
# * Foundation, Inc., 51 Franklin Street, Fifth Floor,                        *
# * Boston, MA  02110-1301  USA                                               *
# *                                                                           *
# * CLooG, the Chunky Loop Generator                                          *
# * Written by Cedric Bastoul, Cedric.Bastoul@inria.fr                        *
# *                                                                           *
# *****************************************************************************/

# This is the main test script of CLooG. It checks that CLooG generates
# a convenient output for an input set of tests, according to some
# parameters (see below). Two checking policies are possible: simply
# compare the generated codes or compare the executions of the generated
# codes. The reference output files must be present: if we are checking a
# file foo.cloog, either foo.c or foo.f must exist in the case of a simple
# code generation checking, and either foo.good.c or foo.good.f must exist
# in the case of a run check.

TEST_NAME="$1"             ## Name of the group of files to test

TEST_FILES="$2"            ## List of test file prefixes and individual options
                           ## spaces between the elements of one test are
                           ## represented with '%', e.g., "file -f -1" is
                           ## "file%-f%-1".

TEST_GLOBAL_OPTIONS="$3"   ## Options for all the tests in the group

TEST_INPUT_EXTENSION="$4"  ## Extension of the input file

TEST_OUTPUT_EXTENSION="$5" ## Extension of the generated file

TEST_TYPE="$6"             ## - "generate" to simply test code generation
                           ##   (default)
                           ## - "regenerate" to replace the original file with
                           ##    the generated one in the case of a check fail
                           ##    !!! USE WITH CARE !!!
                           ## - "valgrind" to test the valgrind output on
                           ##   code generation
                           ## - "run" if the checking policy is to generate,
                           ##   compile and run, generate only otherwise
                           ## - "hybrid" compare source to source and if this
                           ##   fails run them afterwards

failedtest=0;
cloog=$top_builddir/cloog$EXEEXT
echo "             /*-----------------------------------------------*"
echo "              *       Testing CLooG: $TEST_NAME test set       "
echo "              *-----------------------------------------------*/"
for x in $TEST_FILES; do
  name=`echo $x | sed 's/%/ /g' | cut -d\  -f1`;
  individual_options=`echo $x | sed 's/%/ /g' | cut -s -d\  -f2-`;
  input="$srcdir/$name.$TEST_INPUT_EXTENSION";
  output="$srcdir/$name.$TEST_OUTPUT_EXTENSION";
  options="$individual_options $TEST_GLOBAL_OPTIONS";

  echo -e "Check file \x1b[1m$input\x1b[0m \c";
  if [ "$options" = " " ]; then
    echo "(no option), \c"
  else
    echo "(options $options), \c";
  fi;

  if [ "$TEST_TYPE" = "hybrid" ]; then

    echo -e "\x1b[1mTextual checking\x1b[0m";
    $cloog $option -q $input > temp_generated.c
    COMPILE=`echo $COMPILE | sed 's/\\\ /_SPACE_/g'`;
    $COMPILE -P -E temp_generated.c -o temp_generated2.c
    $COMPILE -P -E $output -o temp_generated3.c
    diff -EZwB temp_generated2.c temp_generated3.c > /dev/null
    result=$?
    rm temp_generated.c temp_generated2.c temp_generated3.c

    if [ ! $result -eq 0 ]; then
      echo -e "\x1b[1m\x1b[35mTextual checking failed\x1b[0m";
      echo -e "\x1b[1mComparing execution\x1b[0m";
      generate_test=$srcdir/generate_test_advanced$EXEEXT
      test_run=$srcdir/test_hybrid$EXEEXT
      good="$srcdir/$name.good.$TEST_OUTPUT_EXTENSION";

      $cloog $options -q -callable 1 $input > test_test.c;
      $generate_test $input test_main.c;

      COMPILE=`echo $COMPILE | sed 's/\\\ /_SPACE_/g'`;
      $COMPILE -c test_test.c;
      $COMPILE -Dtest=good -c $good -o test_good.o;
      $LINK test_main.c test_test.o test_good.o > /dev/null;

      $test_run;
      result=$?;
      rm -f $test_run;
    fi

  elif [ "$TEST_TYPE" = "run" ]; then
    generate_test=$srcdir/generate_test_advanced$EXEEXT
    test_run=$srcdir/test_run$EXEEXT
    good="$srcdir/$name.good.$TEST_OUTPUT_EXTENSION";

    echo "generating... \c";
    $cloog $options -q -callable 1 $input > test_test.c;
    $generate_test $input test_main.c;

    echo "compiling... \c";
    # TODO: (nd Cedric) The following line is to deal with the (F*CKING !!!)
    #       space in PACKAGE_STRING, introduced by AC_INIT and which, for
    #       some reason, seems to be the source of a problem with my shell.
    #       Maybe there is a better way to solve the problem...
    COMPILE=`echo $COMPILE | sed 's/\\\ /_SPACE_/g'`;
    $COMPILE -c test_test.c;
    $COMPILE -Dtest=good -c $good -o test_good.o;
    $LINK test_main.c test_test.o test_good.o > /dev/null;

    echo "comparing... \c";
    $test_run;
    result=$?;
    rm -f $test_run;
  elif [ "$TEST_TYPE" = "valgrind" ]; then
    echo "generating... \c";
    #	valgrind --leak-check=full --error-exitcode=1 \
      libtool --mode=execute valgrind --error-exitcode=1 \
      $cloog $options -q $input > /dev/null 2> cloog_temp;
    errors=$?;
    leaks=`grep "in use at exit" cloog_temp | cut -f 2 -d ':'`
    if [ "$errors" = "1" ]; then
      echo -e "\033[31mMemory error detected... \033[0m";
      cat cloog_temp;
      result="1";
    elif [ "$leaks" != " 0 bytes in 0 blocks" ]; then
      echo -e "\033[31mMemory leak detected... \033[0m";
      cat cloog_temp;
      result="1";
    else
      result="0";
    fi;
    rm -f cloog_temp;
  else
    echo "generating... \c";
    $cloog $options -q $input > cloog_temp;
    diff -u -w --ignore-matching-lines='CLooG' $output cloog_temp;
    result=$?;
    if [ "$result" -ne "0" ] && [ "$TEST_TYPE" = "regenerate" ]; then
      echo -e "\033[31mREGENERATING... \033[0m";
      cp cloog_temp $output;
    fi;
    rm -f cloog_temp;
  fi;

  if [ "$result" -ne "0" ]; then
    echo -e "\033[31mFAIL: $output has a problem\033[0m";
    failedtest=`expr $failedtest + 1`;
  else
    echo -e "\x1b[32m$output PASSED\x1b[0m";
  fi;
done;

if [ $failedtest != 0 ]; then
  echo -e "\x1b[31m\x1b[5m[CLooG] FAIL:\x1b[25m $failedtest tests failed in $TEST_NAME\x1b[0m";
else
  echo -e "\x1b[32m[CLooG] PASS: $TEST_NAME passed :-) !\x1b[0m";
fi
exit $failedtest
