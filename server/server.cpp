/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aouhadou <aouhadou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 21:09:18 by aouhadou          #+#    #+#             */
/*   Updated: 2023/09/29 21:23:37 by aouhadou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/headers.hpp"

/***** Class Methodes ****/
int server::_port = 0;
std::string server::_pass = "";

server::server()
{
}

server::server(int port, std::string pass)
{
    _port = port;
    if (_port < 6667 || _port > 6672)
        throw std::invalid_argument("port must be between 6667 and 6672");
    this->_pass = pass;
}

std::string server::get_pass()
{
    return (_pass);
}

int     server::get_port()
{
    return (_port);
}

void    server::set_pass(std::string pass)
{
    this->_pass = pass;
}

void    server::set_port(int port)
{
    this->_port = port;
}

server::server(const server &copy)
{
    *this = copy;
}

server &server::operator=(const server &copy)
{
    if (this != &copy)
    {
        this->_port = copy._port;
        this->_pass = copy._pass;
    }
    return (*this);
}

server::~server()
{
}

/*-------------- Server Functions -----------------*/

int server::request_handler(int i, fd_set *master)
{
    char read_buf[512];
    int bytes_received = recv(i, read_buf, 512, 0);

    if (bytes_received == -1) {
        close(i);
        FD_CLR(i, master);
        client_data.erase(i);
        return -1;
    }
    std::string& client_stream = client_data[i].first;
    client_stream.append(read_buf, bytes_received);

    size_t newline_pos = client_stream.find("\r\n");
    if (newline_pos != std::string::npos) {
        std::string request = client_stream.substr(0, newline_pos);
        request[request.length()] = '\0';

        parse_request((char *)request.c_str(), i, master);
        client_stream.erase(0, newline_pos+1);
        if (client_data[i].second) {
            std::string stored_data = client_data[i].first;
            send(i, stored_data.c_str(), stored_data.size(), 0);
            client_data[i].first.clear();
            client_data[i].second = false;
        }
    }
    if (bytes_received == 0)
        client_data[i].second = true;
    return 1;
}

struct addrinfo *server::get_address()
{
    std::string port;
    struct addrinfo hints;
    std::stringstream ss;
    struct addrinfo *bind_adress;

    ss << _port;
    ss >> port;
    std::cout << "configuring local address..." << std::endl;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(0, port.c_str(), &hints, &bind_adress);
    return (bind_adress);
}

int server::create_socket(struct addrinfo *bind_adress)
{
    int socket_listen;
    std::cout << "Creating a socker..." << std::endl;
    socket_listen = socket(bind_adress->ai_family, bind_adress->ai_socktype, bind_adress->ai_protocol);
    if (socket_listen < 0)
    {
        std::cout << "Creating socket failed!" << std::endl;
        return -1;
    }
    int reuse_addr = 1;
    if (setsockopt(socket_listen, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, sizeof(reuse_addr)) < 0)
    {
        std::cout << "setsockopt(SO_REUSEADDR) failed" << std::endl;
        return -1;
    }
    return (socket_listen);
}

bool    server::bind_and_listen(int socket_listen, struct addrinfo *bind_adress)
{
    std::cout << "Binding socket to local address..." << std::endl;
    if (bind(socket_listen, bind_adress->ai_addr, bind_adress->ai_addrlen))
    {
        freeaddrinfo(bind_adress);
        std::cout << "Binding socket is failed!" << std::endl;
        return false;
    }
    freeaddrinfo(bind_adress);
    std::cout << "listening..." << std::endl;
    if (listen(socket_listen, SOMAXCONN) < 0)
    {
        std::cout << "listen failed!" << std::endl;
        return false;
    }
    return (true);
}

int server::accept_connection(int socket_listen, fd_set *master, int *max_socket)
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
    return(socket_client);
}


void server::run()
{
    int socket_listen;
    struct addrinfo *bind_adress = this->get_address();
    
    if ((socket_listen = create_socket(bind_adress)) < 0)
        return;
    if (!bind_and_listen(socket_listen, bind_adress))
        return;
    if (fcntl(socket_listen, F_SETFL, O_NONBLOCK) < 0)
    {
        std::cout << "fcntl() failed!" << std::endl;
        return;
    }
    fd_set master;
    FD_ZERO(&master);
    FD_SET(socket_listen, &master);
    int max_socket = socket_listen;

    //  set up our call to select():
    std::cout << "Waiting for connections...\n" << std::endl;
    fd_set reads;
    while (true)
    {
        reads = master;

        if (select(max_socket + 1, &reads, 0, 0, 0) < 0){
            std::cout << "select failed!" << std::endl;
            return ;
        }
        for (int i = 0; i <= max_socket; i++) {
            if (FD_ISSET(i, &reads)){
                if (i == socket_listen) {
                    int socket_client = accept_connection(socket_listen, &master, &max_socket);
                    if ( socket_client < 0)
                        continue;
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

