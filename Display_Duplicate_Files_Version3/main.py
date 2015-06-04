import py2
import sys
from PyQt4.QtCore import SIGNAL
from PyQt4 import QtGui, QtCore
def button_click():
	ex = py2.Example(le.text())
	ex.exec_()


app = QtGui.QApplication(sys.argv)
widget = QtGui.QWidget()

widget.setGeometry(200, 100, 500,100 )
widget.setWindowTitle('PyQt Application')

label=QtGui.QLabel(widget)
label.setObjectName("label")
label.setText("Enter Directory")
label.move(50,10)

pb=QtGui.QPushButton(widget)
pb.setObjectName("button")
pb.setText("Find")
pb.move(400,10)

le=QtGui.QLineEdit(widget)
le.setObjectName("text")
le.move(200,10)

pb.clicked.connect(button_click)

widget.show()
sys.exit(app.exec_())
