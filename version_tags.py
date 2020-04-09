from sys import argv
import json

def get_main_version(tag):
    tag_array = tag.split('.')
    if(len(tag_array) < 2):return ''
    new_array = tag_array[0:len(tag_array)-1]
    return '.'.join(new_array)

def insert_into_map(map,newtag):
   # print newtag
    mainversion=get_main_version(newtag)
    if (mainversion == ''): return map
   # print mainversion
    array = map.get(mainversion, [])
    if(array == []):
        map[mainversion] = [newtag]
    else:
        array.append(newtag)
    return map


if __name__ == '__main__':
    filename=argv[1]
    map = {}
    with open(filename) as f1:
        lines = f1.readlines()
        for line in lines:
            line_data = line.split()
            if line_data[0] != '':
                map = insert_into_map(map, line_data[0])
    # print lines
    f1.close()
    print json.dumps(map)
