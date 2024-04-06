import socket, sys, os

def main():
    host = sys.argv[1]
    port = int(sys.argv[2])


    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind((host, port))

    server_socket.listen(5)
    print("Server listening on {}:{}".format(host, port))

    while True:
        
        client_socket, addr = server_socket.accept()
        childPid = os.fork()
        if (childPid == 0):
            print("Connection from:", addr)
            file_name = client_socket.recv(1024).decode()
            print("Received file request:", file_name)

            try:
                with open(file_name, "rb") as file:
                    data = file.read()
                    client_socket.send(data)
                    print("File sent successfully")
            except FileNotFoundError:
                print("File not found")
                client_socket.sendall(b"File not found")

            client_socket.close()

    server_socket.close()

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python server.py [server_address] [port_number]")
    else:
        main()