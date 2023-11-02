#include <iostream>
#include<Network/server/Tcp.h>
//#include <boost/asio.hpp>
//using boost::asio::ip::tcp;
int main(int argc , char* argv[]) {
    MOVF::TcpServer server{ MOVF::IPV::V4 , 1337 };

    server.OnJoin = [](MOVF::Tcp_Connection::pointer server) {
        std::cout << "User has joined the server: " << server->GetUserName() << std::endl;
    };

    server.OnLeave = [](MOVF::Tcp_Connection::pointer server) {
        std::cout << "User has left the server: " << server->GetUserName() << std::endl;
    };

    // idea is to make this T instead of just string
    server.OnClientMessage = [&server](const std::string &message) {
        // Parse the message
        // Do game server things

        // send the message to the client
        server.Broadcast(message);
    };

    server.Run();

    return 0;
}
