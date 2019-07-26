from sys import argv

import json


if __name__ == '__main__':
    filename=argv[1]
    arry = []
    with open(filename) as f1:
        lines = f1.readlines()
        for line in lines:
            line_data = line.split()
            if line_data[0] == "commit" and len(line_data[1]) == 40:
                arry.append( line_data[1] )
    # print lines
    f1.close()

    print json.dumps(arry)