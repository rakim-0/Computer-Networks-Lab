import socket
import os
import signal

def main():
    child1 = 0
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server_socket:
        server_socket.bind(("localhost", 1334))
        server_socket.listen()
        conn, addr = server_socket.accept()
        if conn: 
            print(f"Connected by {addr}")

        
        child2 = os.fork()
        if (child2 == 0):
            while True:
                data = conn.recv(1024)
                if not data:
                    break
                print(f"Received Message: {data.decode("utf-8")}")
                
        child1 = os.fork()
        if (child1 == 0):
            while True:
                try:
                    userInput = input()
                    conn.send(bytes(userInput, 'utf-8'))
                except KeyboardInterrupt:
                    os.kill(child2, signal.SIGTERM)
                    break
        try:
            os.wait()
        except ChildProcessError:
            print("Program Exited Successfully")
if __name__ == "__main__":
    main()
