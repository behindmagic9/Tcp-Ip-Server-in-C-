//
// Created by rajat on 9/20/23.
//

#ifndef TCP_H
#define TCP_H

#include <boost/asio.hpp>
#include <Network/server/Tcp_Connection.h>
#include<optional>
#include<functional>
#include<unordered_set>

namespace MOVF{
    enum class IPV {
        V4,
        V6
    };
    class TcpServer{
        using OnJoinHandler = std::function<void(Tcp_Connection::pointer)>;
        using OnLeaveHandler = std::function<void(Tcp_Connection::pointer)>;
        using OnClientMessageHandler = std::function<void(std::string)>;

    public:
        TcpServer(IPV ipv, int port);
        int Run();
        void Broadcast(const std::string& message);
    private :
        void startAccept();
    public :
        OnJoinHandler OnJoin;
        OnLeaveHandler OnLeave;
        OnClientMessageHandler OnClientMessage;
    private:
        IPV _ipversion;
        int _port;
        boost::asio::io_context _ioContext;
        
        // _acceptor is used to listen for incoming connections on port 12345, and when a connection is established, it accepts it and creates a new socket for communication with the client. 
        boost::asio::ip::tcp::acceptor _acceptor;
        
        
        // std::optional socket with a new boost::asio::ip::tcp::socket object created from the ioContext. The emplace function is used to construct the socket object directly within the std::optional.
        std::optional<boost::asio::ip::tcp::socket> _socket;
        std::unordered_set<Tcp_Connection::pointer> connections {};
    };
}
#endif //NETWORK_BASE_H
