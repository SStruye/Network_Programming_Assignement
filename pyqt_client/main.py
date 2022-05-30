import time
import sys
import zmq
from multiprocessing import Process, Queue
from PySide2.QtWidgets import QApplication, QPushButton, QWidget, QGridLayout, QLabel, QLineEdit, QVBoxLayout, QDialog
from PySide2.QtGui import QFont
from PySide2.QtCore import QTimer


class Game(QWidget):

    def __init__(self, queue):
        super().__init__()
        self.initLogin()

    p1 = ''
    p2 = ''
    topicSend = 'HigherLower>msg?>'
    topicRecv = 'HigherLower>msg!>'
    msg = ''

    def initLogin(self):
        if login().exec_() == QDialog.Accepted:
            self.msg = '>' + self.p1 + '>' + self.p2 + '>'

            self.initUI()

    def initUI(self):
        self.timer = QTimer()
        self.timer.timeout.connect(self.change)
        self.timer.start(1100)

        self.topicSend = self.topicSend + self.p1 + '>' + self.p2 + '>'
        self.topicRecv = self.topicRecv + self.p1 + '>' + self.p2 + '>'

        Process(target=client, args=(self.topicSend + '1>',)).start()
        Process(target=server, args=(q, self.topicRecv)).start()

        grid = QGridLayout()
        self.setLayout(grid)
        grid.setHorizontalSpacing(50)
        grid.setVerticalSpacing(20)

        Font = QFont("Times", 20, QFont.Bold)

        labels = ['SCORE: ', '', 'NUMBER: ', 'SUIT: ']

        # q.qsize().ValueChanged.connect(self.change)

        for x in range(4):
            label = QLabel()
            label.setFont(Font)
            label.setText(labels[x])
            grid.addWidget(label, x, 0)

            self.outScore = QLabel()
            self.outScore.setFont(Font)
            self.outScore.setStyleSheet('border: 1px solid black;')
            grid.addWidget(self.outScore, 0, 1)

            self.outMsg = QLabel()
            # self.outScore.setFont(Font)
            self.outMsg.setStyleSheet('border: 1px solid black;')
            grid.addWidget(self.outMsg, 1, 1)

            self.outMsg2 = QLabel()
            # self.outScore.setFont(Font)
            self.outMsg2.setStyleSheet('border: 1px solid black;')
            grid.addWidget(self.outMsg2, 1, 0)

            self.outNum = QLabel()
            self.outNum.setFont(Font)
            self.outNum.setStyleSheet('border: 1px solid black;')
            grid.addWidget(self.outNum, 2, 1)

            self.outSuit = QLabel()
            self.outSuit.setFont(Font)
            self.outSuit.setStyleSheet('border: 1px solid black;')
            grid.addWidget(self.outSuit, 3, 1)

            bHigher = QPushButton("HIGHER", self)
            bHigher.setFont(Font)
            bHigher.setMinimumWidth(200)
            bHigher.setMinimumHeight(50)
            grid.addWidget(bHigher, 4, 0)
            bHigher.clicked.connect(self.btnHigher)

            bLower = QPushButton("LOWER", self)
            bLower.setFont(Font)
            bLower.setMinimumWidth(200)
            bLower.setMinimumHeight(50)
            grid.addWidget(bLower, 4, 1)
            bLower.clicked.connect(self.btnLower)

            bDisconnect = QPushButton("DISCONNECT", self)
            # bLower.setFont(Font)
            bDisconnect.setMinimumWidth(100)
            bDisconnect.setMinimumHeight(30)
            grid.addWidget(bDisconnect, 0, 2)
            bDisconnect.clicked.connect(self.btnDisconnect)

            bReconnect = QPushButton("RECONNECT", self)
            # bLower.setFont(Font)
            bReconnect.setMinimumWidth(100)
            bReconnect.setMinimumHeight(30)
            grid.addWidget(bReconnect, 1, 2)
            bReconnect.clicked.connect(self.btnReconnect)

            self.outP1 = QLabel()
            self.outP1.setStyleSheet('border: 1px solid black;')
            self.outP1.setText("You: " + self.p1)
            grid.addWidget(self.outP1, 2, 2)

            self.outP2 = QLabel()
            self.outP2.setStyleSheet('border: 1px solid black;')
            self.outP2.setText("Opponent: " + self.p2)
            grid.addWidget(self.outP2, 3, 2)

            self.move(300, 150)
            self.setWindowTitle('Higher Lower')
            self.show()

    def btnHigher(self):
        self.msg = self.topicSend + '3>'
        Process(target=client, args=(self.msg,)).start()

    def btnLower(self):
        self.msg = self.topicSend + '4>'
        Process(target=client, args=(self.msg,)).start()

    def btnDisconnect(self):
        self.msg = self.topicSend + '2>'
        Process(target=client, args=(self.msg,)).start()
        self.outMsg2.setText("You are disconnected")

    def btnReconnect(self):
        self.msg = self.topicSend + '1>'
        Process(target=client, args=(self.msg,)).start()
        self.outMsg2.setText("You are connected")

    def change(self):
        if q.qsize() > 0:
            message = q.get().decode("utf-8")
            li = list(message.split(">"))
            if(li[4] != 'smg'):
                self.outNum.setText(li[4])
                self.outSuit.setText(li[5])
                self.outMsg.setText(li[6])
            else:
                self.outMsg2.setText(li[5])


def server(q, topic):
    context = zmq.Context()
    socket = context.socket(zmq.SUB)
    # socket.bind("tcp://*:5556")
    socket.connect("tcp://benternet.pxl-ea-ict.be:24042")
    socket.setsockopt_string(zmq.SUBSCRIBE, topic)
    print("Running server on port: 5556")
    while(1):
        message = socket.recv()
        while q.full():
            1
        q.put(message)
        print("received: ", message)


def client(msg):
    context = zmq.Context()
    print("Connecting to server with ports 5556")
    socket = context.socket(zmq.PUSH)
    # socket.connect("tcp://localhost:5556")
    socket.connect("tcp://benternet.pxl-ea-ict.be:24041")
    time.sleep(.2)
    print(msg)
    socket.send_string(msg)
    print("send: ", msg)


class login(QDialog):

    def __init__(self):
        super().__init__()
        self.initLogin()

    def initLogin(self):
        layout = QVBoxLayout()
        self.setLayout(layout)
        self.name = QLineEdit()
        self.name.setPlaceholderText("Type your name")
        self.opponent = QLineEdit(self)
        self.opponent.setPlaceholderText("Type your opponents name")
        button = QPushButton("LOGIN", self)
        self.error = QLabel()
        self.error.setText("aint no '>' allowed around here ma boy")
        self.error.setStyleSheet("color: red;")
        self.error.hide()
        layout.addWidget(self.name)
        layout.addWidget(self.opponent)
        layout.addWidget(self.error)
        layout.addWidget(button)
        button.setCheckable(True)
        button.clicked.connect(self.btnstate)

        self.show()

    def btnstate(self):
        try:
            self.name.text().index('>')
        except ValueError:
            try:
                self.opponent.text().index('>')
            except ValueError:
                Game.p1 = self.name.text()
                Game.p2 = self.opponent.text()
                self.accept()
            else:
                self.name.setText("")
                self.opponent.setText("")
                self.error.show()
        else:
            self.name.setText("")
            self.opponent.setText("")
            self.error.show()


if __name__ == '__main__':
    q = Queue()
    app = QApplication(sys.argv)
    ex = Game(q)
    sys.exit(app.exec_())

