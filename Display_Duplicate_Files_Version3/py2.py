import sys,os
from PyQt4.QtCore import SIGNAL
from PyQt4 import QtGui, QtCore
import try5


class Example(QtGui.QWidget):
    
    def __init__(self,text):
        super(Example, self).__init__()
        self.path=text
        self.duplicate=[]
	self.cb=[]
        self.initUI()

    def initUI(self):      
	self.duplicate=try5.Main(str(self.path))
	a = 0
	self.l=len(self.duplicate)
	for i in self.duplicate:
   		self.cb.append(QtGui.QCheckBox(i, self))
		self.cb[a].move(10,15*a)
    		a = a + 1
	de=QtGui.QPushButton(self)
	de.setObjectName("delete")
	de.setText("Delete")
	de.clicked.connect(self.hello)
	de.move(500,500)
        self.setGeometry(100,100,600,600)
        self.setWindowTitle('Event handler')
        self.show()

    def hello(self):
        for i in range(0,self.l):
            if self.cb[i].isChecked():
	          os.remove(self.duplicate[i])
       	        print self.duplicate[i]
