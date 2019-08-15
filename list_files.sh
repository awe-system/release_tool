#!/usr/bin/env bash
tmpdir=$(dirname $0)
cd $tmpdir 2>&1 > /dev/null
tooldir=$(pwd)
cd -  2>&1 > /dev/null
project_dir=$1
ip=$2
base_dir=$3

cd $project_dir  2>&1 > /dev/null
cmd="ls -l ${base_dir}|  grep '^-' | awk '{print \$9}'"
${project_dir}/tool/shell/sshid ${ip} "${cmd}" > list_files
python ${tooldir}/line_to_json.py list_files
cd -  2>&1 > /dev/null