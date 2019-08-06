#!/usr/bin/env bash
tmpdir=$(dirname $0)
cd $tmpdir 2>&1 > /dev/null
tooldir=$(pwd)
cd -  2>&1 > /dev/null
project_dir=$1

cd $project_dir  2>&1 > /dev/null

git tag | grep -v vd > /tmp/gittags
python $tooldir/version_tags.py /tmp/gittags
cd -  2>&1 > /dev/null