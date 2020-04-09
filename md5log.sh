#!/usr/bin/env bash
tmpdir=$(dirname $0)
cd $tmpdir 2>&1 > /dev/null
tooldir=$(pwd)
cd -  2>&1 > /dev/null
project_dir=$1
tag=$2
project=$(cat ${tooldir}/project | tr -d '\n')

cd $project_dir  2>&1 > /dev/null

md5sum /mnt/share/3.project/${project}/log/$tag || exit 1
cd -  2>&1 > /dev/null
