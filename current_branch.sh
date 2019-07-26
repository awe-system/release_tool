#!/usr/bin/env bash
tmpdir=$(dirname $0)
cd $tmpdir 2>&1 > /dev/null
tooldir=$(pwd)
cd -  2>&1 > /dev/null
project_dir=$1

cd $project_dir  2>&1 > /dev/null

git branch | grep '*' | awk '{print $2}' | grep -v ')'
cd -  2>&1 > /dev/null