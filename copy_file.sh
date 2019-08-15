#!/usr/bin/env bash
tmpdir=$(dirname $0)
cd $tmpdir 2>&1 > /dev/null
tooldir=$(pwd)
project_dir=$1
ip=$2
base_dir=$3
filename=$4
local_file=$5

cmd="$project_dir/tool/shell/scpid ${ip}  ${local_file} ${base_dir}/${filename}"
${cmd}
cd -  2>&1 > /dev/null