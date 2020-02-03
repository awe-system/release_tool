#!/usr/bin/env bash
tmpdir=$(dirname $0)
cd $tmpdir 2>&1 > /dev/null
tooldir=$(pwd)
cd -  2>&1 > /dev/null
project_dir=$1

cd $project_dir  2>&1 > /dev/null

$tooldir/current_branch.sh $project_dir > /tmp/gitbranches
git fetch --all 2>&1 > /dev/null

git branch -a |grep -v '*'| grep remotes | awk '{print $1}' >> /tmp/gitbranches
cd -  2>&1 > /dev/null
python $tooldir/show_branch.py /tmp/gitbranches
