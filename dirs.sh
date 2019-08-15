#!/usr/bin/env bash
tmpdir=$(dirname $0)
cd $tmpdir 2>&1 > /dev/null
tooldir=$(pwd)
/usr/bin/cat dirs.json
cd -  2>&1 > /dev/null