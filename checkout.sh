#!/usr/bin/env bash


is_branch_remote()
{
	branch_name=$1
	echo $branch_name | grep 'remotes/origin' >/dev/null && echo "yes"
	echo $branch_name | grep 'remotes/origin' >/dev/null || echo "no"
}


tmpdir=$(dirname $0)
cd $tmpdir 2>&1 > /dev/null
tooldir=$(pwd)
cd -  2>&1 > /dev/null
project_dir=$1
branch_name=$2
cd $project_dir  2>&1 > /dev/null

if [ "$(is_branch_remote ${branch_name})" == "yes" ];then
	git checkout --track ${branch_name:8}
	short_name=${branch_name:15}
	branch_name=${short_name}
fi

git checkout $branch_name || exit 1
git pull 2>&1 > /dev/null 
cd -  2>&1 > /dev/null
