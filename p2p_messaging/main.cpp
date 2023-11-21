#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
int main(int argc, char **argv)
{
    std::cout << "Hello world" << std::endl;

    try
    {
        std::string command = argc >= 2 ? argv[1] : "client";
        if (command == "server")
        {

            boost::asio::io_service io_service;
            tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 12345));
            tcp::socket socket(io_service);
            acceptor.accept(socket);
            printf("Connected from client\n");

            for (;;)
            {
                std::vector<char> buffer;

                // Trying to read from socket
                auto read_available = socket.available();
                if (read_available > 0)
                {
                    boost::asio::read(socket, boost::asio::buffer(buffer.data(), 1024));
                    std::string message(buffer.data(), buffer.size());
                    printf("Message: %s\n");
                }

                std::promise<std::string> stdin_message;
                std::thread stdin_read([&stdin_message]() {
                    
                })
            }
        }
        else if (command == "client")
        {
            std::string host = "127.0.0.1";
            int port = 12345;

            boost::asio::io_service io_service;
            tcp::resolver resolver(io_service);
            tcp::resolver::query query(host, "12345");
            tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

            tcp::socket socket(io_service);
            boost::asio::connect(socket, endpoint_iterator);

            printf("Connected!\n");
            boost::system::error_code error;
            // auto bytes_sent = socket.write_some(boost::asio::buffer("Ping"), error);
            // if (bytes_sent == 0)
            // {
            //     fprintf(stderr, "Error while sending bytes");
            //     exit(1);
            // }
            // printf("Sent %d bytes\n", bytes_sent);

            // char buf[128];
            // auto bytes_received = socket.read_some(boost::asio::buffer(buf), error);
            // printf("Received!\n");
            // if (error == boost::asio::error::eof)
            // {
            //     printf("Closed by remote");
            // }
            // else if (error)
            // {
            //     throw boost::system::system_error(error);
            // }
            // printf("Received %d bytes\n", bytes_received);

            // std::cout.write(buf, bytes_received);

            for (;;)
            {
                std::string message;

                std::getline(std::cin, message);
                auto bytes_sent = socket.write_some(boost::asio::buffer(message));
                printf("Sent %d bytes\n", bytes_sent);
                auto bytes_received = socket.read_some(boost::asio::buffer(message));
                printf("Received %d bytes, message: %s", bytes_received, message);
            }
        }
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}