#!/usr/bin/env bash
tmpdir=$(dirname $0)
cd $tmpdir 2>&1 > /dev/null
tooldir=$(pwd)
cd -  2>&1 > /dev/null
project_dir=$1

cd $project_dir  2>&1 > /dev/null
python $tooldir/line_to_json.py /mnt/share/3.project/iceberg/deploy.xml.d/service_dir
cd -  2>&1 > /dev/null