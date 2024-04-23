import socket
import sys

def main():
    if len(sys.argv) != 4:
        print("Usage: python client.py [server_address] [server_port] [command]")
        return

    server_address = sys.argv[1]
    server_port = int(sys.argv[2])
    command = sys.argv[3]
    if (command.split(maxsplit=1)[0] != 'execute'):
        print('command should be of format: execute <program>')
        print('example: execute ls')
        exit(1)
    
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    try:
        client_socket.sendto(command.encode(), (server_address, server_port))
        data,_ = client_socket.recvfrom(4096)
        print(data.decode())
    except Exception as e:
        print("Error:", e)

    client_socket.close()

if __name__ == "__main__":
    main()
