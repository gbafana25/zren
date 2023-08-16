#!/usr/bin/env python3
import socket
import os
import sys

def startServer():
    soc = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    soc.bind(('127.0.0.1', 8010))
    soc.listen()
    con, ad = soc.accept()
    data = con.recv(512)
    str_data = data.decode('utf-8')
    tar_info = getRequestData(str_data)
    tar_size = tar_info[0]
    tar_dir = tar_info[1]+'.tar.gz'
    #print(tar_size)

    # send ready-to-recieve signal
    con.send(b"ready") 
    tarball = con.recv(tar_size)
    with open(tar_dir, "wb+") as tar:
        tar.write(tarball)
        tar.close()

    #print(len(tarball))
    
    


def getRequestData(data):
    info_array = data.split(' ')
    commit_id = info_array[0]
    branch = info_array[1]
    print(info_array)
    try:
        os.mkdir('branches/'+branch)
        os.mkdir('branches/'+branch+'/'+commit_id)
    except:
            print("directories already exist")

    return (int(info_array[5]), 'branches/'+branch+'/'+commit_id) 

def setupDir():
    try:
        os.mkdir('logs')
        os.mkdir('branches')
    except:
            print("directories already exist")


if __name__ == "__main__":
    if len(sys.argv) >= 2:
            if sys.argv[1] == "setup":
                setupDir()
    else:
        startServer()

