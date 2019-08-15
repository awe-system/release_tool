#!/usr/bin/env bash
tmpdir=$(dirname $0)
cd $tmpdir 2>&1 > /dev/null
tooldir=$(pwd)
project_dir=$1
ip=$2
service=$3
map_service=$(/usr/bin/python service_map.py ${service})
conf_cmd=$(/usr/bin/python service_get_conf.py ${service})
remotecmd="/usr/bin/python /root/${map_service}_tool.py ${conf_cmd}"
echo "${remotecmd}"
cmd="$project_dir/tool/shell/sshid ${ip} \"${remotecmd}\""
echo "${cmd}"
$project_dir/tool/shell/sshid ${ip} "${remotecmd}"

cd -  2>&1 > /dev/null