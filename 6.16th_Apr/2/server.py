import socket
import subprocess
import shlex 

def main():
    UDP_IP = "192.168.199.172" 
    UDP_PORT = 8080 

    server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    server_socket.bind((UDP_IP, UDP_PORT))

    print("Server listening on {}:{}".format(UDP_IP, UDP_PORT))

    while True:
        data, addr = server_socket.recvfrom(1024)
        print("Received command from client:", data.decode())
        # command = data.decode().split(maxsplit=1)[1]
        command = shlex.split(data.decode())[1:]

        try:
            output = subprocess.check_output(command, stderr=subprocess.STDOUT)
            # print(output)
            server_socket.sendto(output, addr) 
        except FileNotFoundError:
            server_socket.sendto(b"Error: The specified executable does not exist.", addr)
        except subprocess.CalledProcessError as e:
            server_socket.sendto(e.output, addr)

    server_socket.close()

if __name__ == "__main__":
    main()
