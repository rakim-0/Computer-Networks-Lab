import socket
import os
import signal
def main():

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as c:
        c.connect(("localhost", 1334))
        
        child1 = os.fork()
        if (child1 == 0):
            while True:
                data = c.recv(1024)
                if not data:
                    break
                print(f"Received Message: {data.decode("utf-8")}")

        child2 = os.fork()
        if (child2 == 0):
            while True:
                try:
                    userInput = input()
                    c.send(bytes(userInput, 'utf-8'))
                except KeyboardInterrupt:
                    break
            os.kill(child1, signal.SIGTERM)
    try:
        os.wait()
    except ChildProcessError:
        print("Program Exited Successfully")
        

if __name__ == "__main__":
    main()
