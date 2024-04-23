import socket
import sys

def main():
    host = sys.argv[1]
    port = int(sys.argv[2])
    file_name = sys.argv[3]

    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    client_socket.connect((host, port))

    client_socket.sendall(file_name.encode())

    data = client_socket.recv(1024)
    if data == b"File not found":
        print("File not found on server")
    else:
        with open("receive.txt", "wb") as file:
            file.write(data)
            print("File received successfully")
    client_socket.close()

if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("Usage: python client.py [server_address] [port_number] [file_name.txt]")
    else:
        main()