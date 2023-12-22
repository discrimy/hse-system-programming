//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2023 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <array>
#include <boost/asio.hpp>
#include <ctime>
#include <iostream>
#include <string>

using boost::asio::ip::udp;

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Usage: %s <receive port> <send port>\n", argv[0]);
    return 1;
  }
  int receive_port = std::atoi(argv[1]);
  int send_port = std::atoi(argv[2]);

  bool working = true;

  auto receive_thread = std::thread([&working, receive_port]() {
    try {
      boost::asio::io_context io_context;

      udp::socket socket(io_context, udp::endpoint(udp::v4(), receive_port));
      while (working) {
        std::array<char, 1024> recv_buf;
        udp::endpoint remote_endpoint;

        boost::asio::socket_base::bytes_readable command(true);
        socket.io_control(command);
        std::size_t bytes_readable = command.get();
        if (bytes_readable > 0) {

          socket.receive_from(boost::asio::buffer(recv_buf), remote_endpoint);
          std::string in_message(recv_buf.data());
          std::cout << "Received message: " << in_message << std::endl;
        }
      }
    } catch (std::exception &e) {
      std::cerr << e.what() << std::endl;
    }
  });

  auto send_thread = std::thread([&working, send_port]() {
    try {
      boost::system::error_code error;
      boost::asio::io_context io_context;

      udp::socket socket(io_context);
      socket.open(udp::v4(), error);
      if (error) {
        std::cerr << "Error while open socket!" << std::endl;
        return;
      }
      socket.set_option(udp::socket::reuse_address(true));
      socket.set_option(boost::asio::socket_base::broadcast(true));
      udp::endpoint endpoint(boost::asio::ip::address_v4::broadcast(),
                             send_port);

      while (working) {
        std::string input;
        std::getline(std::cin, input);
        if (input == "!quit") {
          working = false;
        } else {
          std::cout << "Send messsage: " << input << std::endl;

          socket.send_to(boost::asio::buffer(input), endpoint);
        }
      }
    } catch (std::exception &e) {
      std::cerr << e.what() << std::endl;
    }
  });

  printf("Type message to send.\n");
  printf("Type '!quit' to exit.\n");

  receive_thread.join();
  send_thread.join();

  return 0;
}