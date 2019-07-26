#!/usr/bin/env bash
tmpdir=$(dirname $0)
cd $tmpdir 2>&1 > /dev/null
tooldir=$(pwd)
cd -  2>&1 > /dev/null
project_dir=$1
branch_name=$2

cd $project_dir  2>&1 > /dev/null

git checkout $branch_name || exit 1
cd -  2>&1 > /dev/null