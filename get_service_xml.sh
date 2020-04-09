#!/usr/bin/env bash
tmpdir=$(dirname $0)
cd $tmpdir 2>&1 > /dev/null
tooldir=$(pwd)
cd -  2>&1 > /dev/null
project_dir=$1
service_name=$2
project=$(cat ${tooldir}/project | tr -d '\n')

cd $project_dir  2>&1 > /dev/null
ls /mnt/share/3.project/${project}/deploy.xml.d/${service_name}/*.xml -l | awk '{print $9}' | cut -d '/' -f8 | cut -d '.' -f1 > ${tooldir}/whole_xml
python $tooldir/line_to_json.py ${tooldir}/whole_xml
cd -  2>&1 > /dev/null
