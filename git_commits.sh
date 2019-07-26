#!/usr/bin/env bash
tmpdir=$(dirname $0)
cd $tmpdir 2>&1 > /dev/null
tooldir=$(pwd)
cd -  2>&1 > /dev/null
project_dir=$1

cd $project_dir  2>&1 > /dev/null
git log  | grep 'commit ' > /tmp/gitcommits
cd -  2>&1 > /dev/null
python $tooldir/show_commit.py /tmp/gitcommits