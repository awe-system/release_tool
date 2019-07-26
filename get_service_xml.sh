#!/usr/bin/env bash
tmpdir=$(dirname $0)
cd $tmpdir 2>&1 > /dev/null
tooldir=$(pwd)
cd -  2>&1 > /dev/null
project_dir=$1
service_name=$2

cd $project_dir  2>&1 > /dev/null
ls /mnt/share/3.project/iceberg/deploy.xml.d/${service_name}/*.xml -l | awk '{print $9}' | cut -d '/' -f8 | cut -d '.' -f1 > /tmp/whole_xml
python $tooldir/line_to_json.py /tmp/whole_xml
cd -  2>&1 > /dev/null