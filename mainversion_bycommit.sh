#!/usr/bin/env bash
tmpdir=$(dirname $0)
cd $tmpdir 2>&1 > /dev/null
tooldir=$(pwd)
cd -  2>&1 > /dev/null
project_dir=$1
commit=$2
branch=$(sh current_branch.sh ${project_dir})

cd $project_dir  2>&1 > /dev/null
git checkout ${commit} version  2>&1 > /dev/null
cat version
git checkout ${branch} version  2>&1 > /dev/null
cd -  2>&1 > /dev/null
