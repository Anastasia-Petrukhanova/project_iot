from flask import Flask, request
from datetime import datetime
import socket, threading

HOST = ""  # localhost
PORT =     # Port to listen on

class TCPServerMulti:
    def __init__(self, host, port):
        self.host = host
        self.port = port
        self.sock = None
        self.conn_arduino = None
        self.a = True
    def printwt(self, msg):
        current_date_time = datetime.now().strftime('%d:%m:%Y %H:%M:%S')
        print(f'[{current_date_time}] {msg}')
    def configure_server(self):
        self.printwt('Creating socket...')
        self.sock = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
        self.printwt('Socket created')
        self.printwt(f'Binding server to {self.host}:{self.port}')
        self.sock.bind((self.host,self.port))
        self.printwt(f'Server binded to {self.host}:{self.port}')
    def wait_for_client(self):
        try:
            self.printwt('Listening for incoming connection...')
            self.sock.listen(10)
            while True:
                client_sock, client_address = self.sock.accept()
                self.printwt(f'Accepted connection from {client_address}')
                self.conn_arduino = client_sock
                c_thread = threading.Thread(target=self.handle_client,args=(client_sock,client_address))
                c_thread.start()
        except KeyboardInterrupt:
            self.shutdown_server()
    def handle_client(self, client_sock, client_addres):
        try:
            while True:
                a = 2
            self.printwt(f'Connection closed by {client_addres}')
        except OSError as err:
            self.printwt(err)
        finally:
            self.printwt(f'Closing client socket for {client_addres}')
            client_sock.close()
            self.printwt(self.u_sock)
            self.printwt(f'Client socket closed for {client_addres}')
    def getArduino(self, message):
        self.conn_arduino.send(message.encode())
        data = self.conn_arduino.recv(1024).decode()
        return data

app = Flask(__name__)
  
tcp_server = TCPServerMulti(HOST,PORT)
@app.route('/start')
def tcp_con():
    global tcp_server
    tcp_server = TCPServerMulti(HOST,PORT)   
    tcp_server.configure_server()
    tcp_server.wait_for_client()
    return {
        "m": "ok"
    }


@app.route('/level')
def level():
    data = tcp_server.getArduino("dis\n")
    return {
        "value": data,
        "massege": "Уровень снега (в см): "
    }
  
@app.route('/tmp')
def tmp():
    data = tcp_server.getArduino("tmp\n")
    return {
        "value": data,
        "massege": "Температура в гараже (в C): "
    }

@app.route('/tmp-val', methods = ["POST"])
def tmpVal():
    val = request.get_json()
    m = "val"+str(val["value"])+"\n"
    data = tcp_server.getArduino(m)
    return 'Done', 201

@app.route('/tmp-val1')
def tmpValDone():
    return {
        "value": '',
        "massege": "Температура установлена"
    }
      

if __name__ == '__main__':
    app.run(debug=True, threaded=True)
    
