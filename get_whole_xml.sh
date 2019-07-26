#!/usr/bin/env bash
tmpdir=$(dirname $0)
cd $tmpdir 2>&1 > /dev/null
tooldir=$(pwd)
cd -  2>&1 > /dev/null
project_dir=$1

cd $project_dir  2>&1 > /dev/null
ls /mnt/share/3.project/iceberg/deploy.xml.d/*.xml -l | awk '{print $9}' | cut -d '/' -f7 | cut -d '.' -f1> /tmp/whole_xml
python $tooldir/line_to_json.py /tmp/whole_xml
cd -  2>&1 > /dev/null