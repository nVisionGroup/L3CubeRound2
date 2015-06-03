import sys
argv=len(sys.argv)
argc=""
option=""
if(argv==1):
		print "Usage:"
		print( "svc FILENAME \tcommit file");
		print("svc N \t display nth version of the file");
		print("svc will track the versions of one file for you.");
		exit(0);
elif(argv==2):
	if(sys.argv[1].isdigit()):
		option="VERSION"
	else:
		option="COMMIT"	
argc=sys.argv[1]  	#filename or version number
if(option=="COMMIT"):      # svc filename
	f=open(argc,"r+")
	meta=open("meta","r")
	a=meta.readlines()
	print a
	f.seek(0)
	print "Appending"
	data=f.readlines()
	count=0
	meta.close()
	meta=open("meta","a")
	if(len(a)!=0):
		for i in a[-2]:	#counts the lines of current and previous version 
			if(i==" "):
				count+=1 			
		if(len(data)==count-1):	#current and previous lines are same 
			print "Already commited"
			exit()
	else:
		meta.write(" ")		
	for i in range(len(data)):
		data[i]=data[i][0:len(data[i])-1]
	data.append("\n")
	print data	

	for i in data:
		meta.write(i+" ")
	meta.close()
	f.close()
else:
	meta=open("meta","r")
	version=int(argc)-1
	
	for i ,line in enumerate(meta):
		if(version==i):
			print line
			break
	meta.close()
