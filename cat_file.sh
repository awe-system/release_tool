#!/usr/bin/env bash
tmpdir=$(dirname $0)
cd $tmpdir 2>&1 > /dev/null
tooldir=$(pwd)
cd -  2>&1 > /dev/null
project_dir=$1
ip=$2
base_dir=$3
filename=$4

cd $project_dir  2>&1 > /dev/null
cmd="/usr/bin/cat ${base_dir}/${filename}"
${project_dir}/tool/shell/sshid ${ip} "${cmd}"
cd -  2>&1 > /dev/null