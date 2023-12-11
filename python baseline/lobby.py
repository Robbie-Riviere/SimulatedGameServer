# Import Statements:
import client
import server


# Global Variables
port = 6969


# Lobby connecting Server and Client:
def create_lobby():

    # Main menu:
    print("Welcome! Choose an option:")
    print("1. Be a Server")
    print("2. Be a Client")

    while True:
        choice = input("Enter 1 or 2: ")
        if choice == '1' or choice == '2':
            print(); break
        else:
            print("Invalid Option!")

    # Server Side Actions:
    if choice == "1":
        server.host_server()

    # Client Side Actions:
    elif choice == "2":
        valid_servers = client.search_for_servers()
        print("Search complete! Valid servers:", valid_servers)


# Lobby connecting Server and Client:
def main():
    create_lobby()


if __name__ == "__main__":
    main()
