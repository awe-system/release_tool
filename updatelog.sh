#!/usr/bin/env bash
tmpdir=$(dirname $0)
cd $tmpdir 2>&1 > /dev/null
tooldir=$(pwd)
cd -  2>&1 > /dev/null
project_dir=$1
tag=$2

cd $project_dir  2>&1 > /dev/null

cat /mnt/share/3.project/iceberg/log/$tag || exit 1
cd -  2>&1 > /dev/null