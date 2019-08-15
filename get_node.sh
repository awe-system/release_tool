#!/usr/bin/env bash
tmpdir=$(dirname $0)
cd $tmpdir 2>&1 > /dev/null
tooldir=$(pwd)
cd -  2>&1 > /dev/null
project_dir=$1
xml=$2

ln -sf ${project_dir}/tool/deploy/load_xml.py $tooldir/load_xml.py
cd $tooldir  2>&1 > /dev/null
python $tooldir/all_ip_array.py ${project_dir} ${xml}
cd -  2>&1 > /dev/null
