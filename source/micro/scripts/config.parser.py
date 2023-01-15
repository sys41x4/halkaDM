#!/usr/bin/python3
import sys, toml, base64
import subprocess as sp
default_config_filePath = '/etc/halkaDM/halkaDM.config'
try:
    default_config_filePath = sys.argv[2]
except:
    pass


def exec(cmd):
    #cmd = cmd.split(" ")
    # print(cmd)
    #with sp.Popen(cmd, stdout=sp.PIPE, bufsize=1, universal_newlines=True) as p:
    #    for line in p.stdout:
    #        print(line, end='') # process line here
    print(sp.getoutput(cmd))
    #if p.returncode != 0:
    #    raise sp.CalledProcessError(p.returncode, p.args)


try:
    fetchID = sys.argv[1].split('@')
    with open(default_config_filePath, 'r') as f:data = toml.loads(f.read())[fetchID[0]][fetchID[1]]
    try:
        # print(base64.b64decode(data).decode())
        exec(base64.b64decode(data).decode())
    except:
        print(data)
except KeyError:
    print("VALUE NOT FOUND")
except:
    print(f"SYNTAX: {sys.argv[0]} <SECTION@KEY> <CONFIG_FILEPATH>")
