#!/usr/bin/env bash
tmpdir=$(dirname $0)
cd $tmpdir 2>&1 > /dev/null
tooldir=$(pwd)
project=$(cat ${tooldir}/project | tr -d '\n')
cd -  2>&1 > /dev/null
project_dir=$1
xml=/mnt/share/3.project/${project}/deploy.xml.d/${2}.xml
bugname=$3
cd $project_dir/tool/collect  2>&1 > /dev/null
echo "${xml} " > ${project_dir}/collect.log
echo "${bugname} " >> ${project_dir}/collect.log
bash collect.sh ${xml} ${bugname} >> ${project_dir}/collect.log &
cd -  2>&1 > /dev/null
