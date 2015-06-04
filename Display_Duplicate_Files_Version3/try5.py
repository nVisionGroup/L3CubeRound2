import os,sys
import hashlib
import threading


data={}
lock=threading.Lock()

def hashkey(path):
    try:
    	file_1 = open(path, 'rb')
    	hashe = hashlib.md5()
    	buf = file_1.read(65536)
    	while len(buf) > 0:
        	hashe.update(buf)
        	buf = file_1.read(65536)
    	file_1.close()
    	return hashe.hexdigest()
    except IOError:
	return -1

def filescan(directory,data):
	data1={}
	for dir,subdirs,files in os.walk(directory):
		for file in files:
			temp=os.path.join(dir,file)
			key=hashkey(temp)
			if key in data1:
				data1[key].append(temp)
			elif key != -1:
				data1[key]=[temp]
	lock.acquire()
	merge(data1)
	lock.release()

def merge(data1):
	for key in data1.keys():
        	if key in data.keys():
            		data[key] = data[key] + data1[key]
        	else:
            		data[key] = data1[key]

 
def starting(path):
	t=[]
	i=0
	files = [f for f in os.listdir(path) if os.path.isfile(os.path.join(path,f))]  # files only	
	for file in files:
		temp=os.path.join(path,file)
		key=hashkey(temp)
		if key in data:
			data[key].append(temp)
		elif key != -1:
			data[key]=[temp]
	subdirs = [d for d in os.listdir(path) if os.path.isdir(os.path.join(path,d))]  # directoy only	
	for dirs in subdirs:
		t.append(threading.Thread(target=filescan,args=(os.path.join(path,dirs),data)))
		t[i].start()
		i=i+1
	for a in range(i):
		t[a].join()
def Input():
	path=raw_input("Enter Path of directory :	")
	if not os.path.isdir(path):
		Input()
	else:
		return path
	
def Main(path):
	duplicate=[]
	starting(path)
	for x in data:
		if len(data[x]) > 1:
			duplicate=duplicate+data[x]

	i=1
	for x in duplicate:
		print i, x
		i=i+1
	return duplicate
