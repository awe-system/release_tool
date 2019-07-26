#!/usr/bin/env bash
tmpdir=$(dirname $0)
cd $tmpdir 2>&1 > /dev/null
tooldir=$(pwd)
cd -  2>&1 > /dev/null
project_dir=$1

cd $project_dir  2>&1 > /dev/null

bash release.sh > /dev/null || exit 1
cat cur_tag
cd -  2>&1 > /dev/null