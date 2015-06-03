Problem Definition:- Simple version controlling
Author:-
	nVision Group
Assumption:-
GOAL:
Create a simple version control (svc) program called "svc".
DETAILS:
We have a text file that needs version control i.e., ability to revert back
to any previous version.  
- The text that is composed of one or more lines.
- Each line has a maximum character width of 10 characters (including newline).
- The total number of lines is 20.
The following operations are permitted:
1. Appending a new line at the end of the file.
2. Deleting any existing line.
Only one of the above operations can be done at a given time i.e., the user can either append a line -or- delete a line. After each operation, the file is commited using the svc. 
The usage of svc is the following
svc filename   : To commit
svc N          : Output Nth version of the file.

Idea:-
Here we are not using multiple files, each one for all different versions.
It would have contained un-necessary multiple copies of same data.
We take advantage of the fact that difference between two.
Here we are not using multiple files, each one for all different versions.
It would  have contained un-necessary multiple copies of same data.
We take the advantage of the fact that difference between two versions will always be the only one operation. So we are keeping track of difference between previous version and current version only.
For that, we have maintained an extra metadata file and in that file we insert the required information about difference between two versions
 



