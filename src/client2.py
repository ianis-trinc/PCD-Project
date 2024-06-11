import socket
import sys
import select

SERVER_IP = "127.0.0.1"
PORT = 8080

def handle_client(client_type, username):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((SERVER_IP, PORT))
        s.setblocking(0)
        print(f"Connected to the server as {username} ({client_type})")
        init_message = f"{client_type}:{username}"
        s.sendall(init_message.encode())

        while True:
            sockets_list = [sys.stdin, s]
            read_sockets, write_socket, error_socket = select.select(sockets_list, [], [])

            for socks in read_sockets:
                if socks == s:
                    message = socks.recv(1024)
                    if not message:
                        print("Server closed connection.")
                        return
                    print(f"Server: {message.decode()}")
                else:
                    message = sys.stdin.readline()
                    s.send(message.encode())
                    if message.strip() == "shutdown":
                        print("You do not have permission to shut down the server.")
                    if message.strip() == "exit":
                        return

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print(f"Usage: {sys.argv[0]} <client_type> <username>")
        sys.exit(1)
    
    client_type = sys.argv[1]
    username = sys.argv[2]
    handle_client(client_type, username)
