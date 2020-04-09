#!/usr/bin/env bash
tmpdir=$(dirname $0)
cd $tmpdir 2>&1 > /dev/null
tooldir=$(pwd)
cd -  2>&1 > /dev/null
project_dir=$1
project=$(cat ${tooldir}/project | tr -d '\n')

cd $project_dir  2>&1 > /dev/null
ls /mnt/share/3.project/${project}/deploy.xml.d/*.xml -l | awk '{print $9}' | cut -d '/' -f7 | cut -d '.' -f1> ${tooldir}/whole_xml
python $tooldir/line_to_json.py ${tooldir}/whole_xml
cd -  2>&1 > /dev/null
