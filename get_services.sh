#!/usr/bin/env bash
tmpdir=$(dirname $0)
cd $tmpdir 2>&1 > /dev/null
tooldir=$(pwd)
project=$(cat ${tooldir}/project | tr -d '\n')
cd -  2>&1 > /dev/null
project_dir=$1

cd $project_dir  2>&1 > /dev/null
python $tooldir/line_to_json.py /mnt/share/3.project/${project}/deploy.xml.d/service_dir
cd -  2>&1 > /dev/null
