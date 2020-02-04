#!/usr/bin/env bash
tmpdir=$(dirname $0)
cd $tmpdir  > /dev/null 2>&1 
tooldir=$(pwd)
cd - > /dev/null  2>&1
project_dir=$1

cd $project_dir > /dev/null 2>&1

bash release.sh > /dev/null 2>&1 || exit 1
cat cur_tag
cd - > /dev/null 2>&1 
