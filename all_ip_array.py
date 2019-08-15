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


def load_filters():
    file_json_path = "service.json"
    str = load_str(file_json_path)
    filters = json.loads(str)
    return filters

def echo_random(arg):
    os.system(shelldir + "/echo_color random \"" + arg + "\"")


def echo_diff(arg):
    os.system(shelldir + "/echo_color diff \"" + arg + "\"")


def service_save_path(bugnum):
    date = commands.getoutput("date \"+%Y%m%d%H%M\"")
    path = "/mnt/log/" + bugnum + "/" + date + "/"
    return path


def ok_path(service_path, service_name, ip):
    return service_path + "/" + service_name + "/" + ip + "/ok"


def check_remove_waitlist(waitlist, service_path):
    for array in waitlist:
        service_name = array[0]
        ip = array[1]
        if( os.path.exists(ok_path(service_path, service_name, ip))):
            waitlist.remove(array)
            echo_diff("-----------" + service_name + " " + ip + "------ok")
            break
    return waitlist

if __name__ == '__main__':
    if (len(argv) < 2): help();exit(1)
    projectdir = argv[1]
    xml_path = argv[2]
    # print(in_service_name,sys.stderr)
    shelldir = projectdir + "/tool/shell"
    # print projectdir
    # print xml_path
    # print  out_service_name
    nodes, services, smoke = lx.load_xml(xml_path)
    service_names = []
    res = []
    for node,nodeitem in nodes.items():
        res.append(nodeitem['ip'])
    print json.dumps(res)
