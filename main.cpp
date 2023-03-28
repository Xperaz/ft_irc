#include "ft_irc.hpp"

/*** implmentation of irc server side ****/
struct addrinfo *server::get_address()
{
    std::string port;
    struct addrinfo hints;
    std::stringstream ss;
    struct addrinfo *bind_adress;

    ss<< _port;
    ss >> port;
    std::cout << "configuring local adress..." << std::endl;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(0, port.c_str(), &hints, &bind_adress);
    return (bind_adress);
}

int create_socket(struct addrinfo *bind_adress)
{
    int socket_listen;
    std::cout << "Creating a socker..." << std::endl;
    socket_listen = socket(bind_adress->ai_family, bind_adress->ai_socktype, bind_adress->ai_protocol);
    if (socket_listen < 0)
    {
        std::cout << "Creating socket failed!" << std::endl;
        return -1;
    }
    return (socket_listen);
}

bool    bind_and_listen(int socket_listen, struct addrinfo *bind_adress)
{
    std::cout << "Binding socket to local address..." << std::endl;
    if (bind(socket_listen, bind_adress->ai_addr, bind_adress->ai_addrlen))
    {
        std::cout << "Binding socket is failed!" << std::endl;
        return false;
    }
    freeaddrinfo(bind_adress);
    std::cout << "listening..." << std::endl;
    if (listen(socket_listen, 10) < 0)
    {
        std::cout << "listen failed!" << std::endl;
        return false;
    }
    return (true);
}

int accept_connection(int socket_listen, fd_set *master, int *max_socket)
{
    struct sockaddr_storage client_adress;
    socklen_t client_len = sizeof(client_adress);
    int socket_client = accept(socket_listen, (struct sockaddr*) &client_adress, &client_len);
    if (socket_client < 0)
    {
        std::cout << "accept failed!" << std::endl;
        return -1;
    }
    FD_SET(socket_client, master);
    if (socket_client > *max_socket)
        *max_socket = socket_client;
    char address_buffer[100];
    getnameinfo((struct sockaddr*)&client_adress, client_len, address_buffer, sizeof(address_buffer), 0, 0, NI_NUMERICHOST);
    std::cout << "New connection from: " << address_buffer << std::endl;
    return(1);
}

int request_handler(int i, fd_set *master)
{
    char read[512];
    int bytes_received = recv(i, read, 512, 0);
    if (bytes_received < 1){
        FD_CLR(i, master);
        close(i);
        return (-1);
    }

    for (int j = 0; j < bytes_received; j++)
        read[j] = toupper(read[j]);
    send(i, read, bytes_received, 0);
    return (1);
}

void server::run()
{
    int socket_listen;
    struct addrinfo *bind_adress = this->get_address();
    
    if ((socket_listen = create_socket(bind_adress)) < 0)
        return;
    if (!bind_and_listen(socket_listen, bind_adress))
        return;

    fd_set master;
    FD_ZERO(&master);
    FD_SET(socket_listen, &master);
    int max_socket = socket_listen;

    //  set up our call to select():
    std::cout << "Waiting for connections...\n" << std::endl;
    while (true)
    {
        fd_set reads;
        reads = master;
        if (select(max_socket +1, &reads, 0, 0, 0) < 0){
            std::cout << "select failed!" << std::endl;
            return;
        }
        for (int i = 0; i <= max_socket; i++) {
            if (FD_ISSET(i, &reads)){
                if (i == socket_listen) {
                    if (accept_connection(socket_listen, &master, &max_socket) < 0)
                        return;
                }
                else{
                    if (request_handler(i, &master) < 0)
                        continue;
                }
            }
        } 
    }
    close(socket_listen);
    return;
}

int main(void)
{
    server s(6667);
    s.run();
    return 0;
}