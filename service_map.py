#!/usr/bin/python2.7
import json
import os
import commands
from sys import argv
import load_xml as lx
import sys

def load_str(finename):
    f = open(finename, "r")
    str = f.read()
    f.close()
    return str


def load_map():
    file_json_path = "service.json"
    str = load_str(file_json_path)
    map = json.loads(str)
    return map

if __name__ == '__main__':
    if (len(argv) < 2): help();exit(1)
    servicename = argv[1]
    cmd_map = load_map()
    print(cmd_map[servicename])
