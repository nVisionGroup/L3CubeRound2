import sys

argv=len(sys.argv)  #count the commandline arguments
argc=""
option=""
if(argv==1):   #if user does not give any file name or version number i.e. simpely  python svc_v1.py
		print "Usage:"
		print( "svc FILENAME \tcommit file");
		print("svc N \t display nth version of the file");
		print("svc will track the versions of one file for you.");
		exit(0);
elif(argv==2):
	if(sys.argv[1].isdigit()):  #check whether argument is version number or filename 
		option="VERSION"
	else:
		option="COMMIT"	
argc=sys.argv[1]  	#filename or version number
if(option=="COMMIT"):      # svc filename
	f=open(argc,"r+")    #open input file in read mode
	meta=open("meta","r")  #open metadata file read mode
	a=meta.readlines()
	print a
	f.seek(0)
	print "Appending"
	data=f.readlines()   
	count=0
	meta.close()   #close the metadata file 
	meta=open("meta","a")  #open metadata file in append mode
	if(len(a)!=0):
		for i in a[-2]:	#counts the lines of current and previous version 
			if(i==" "):
				count+=1 			
		if(len(data)==count-1):	 
			print "Already commited"
			exit()
	else:				#if new line is append or delete
		meta.write(" ")		
	for i in range(len(data)):	
		data[i]=data[i][0:len(data[i])-1]  #check current version data and previous versin data.
	data.append("\n")
	print data	

	for i in data:
		meta.write(i+" ")
	meta.close()		#close the metadata file.
	f.close()		#close the actual data file i.e. test.txt 
else:			#if option is to display perticular version data
	meta=open("meta","r")	#open metadata file in read mode
	version=int(argc)	
	
	for i ,line in enumerate(meta):  #assign version number to each line present n metadata file 
		if(version==i):		#check input version number in metadata file
			print line	#print data of that perticular input version number
			break
	meta.close()			#close the metadata file
