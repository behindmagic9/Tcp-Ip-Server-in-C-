//
// Created by rajat on 9/20/23.
//
#include<Network/server/Tcp.h>
#include<iostream>

namespace MOVF{
    using boost::asio::ip::tcp ;
    TcpServer::TcpServer(IPV ipv,int port)  : _ipversion(ipv) , _port(port) , _acceptor (_ioContext,tcp::endpoint(_ipversion == IPV::V4 ? tcp::v4() : tcp::v6(), _port)) {
    // _ioContext) to manage asynchronous operations associated with the acceptor.

    }

    int TcpServer::Run(){   // run all iocontext
        try{
            startAccept();
            _ioContext.run(); //blocking io
        }catch(std::exception &e){
            std::cerr << e.what() << std::endl;
            return -1;
        }
        return 0;
    }
    void TcpServer::Broadcast(const std::string &message){
        for(auto& _connections : connections){
            _connections->Post(message);
        }
    }

    void TcpServer::startAccept(){
        _socket.emplace(_ioContext);


//  asynchronous operation to accept an incoming TCP connection using the _acceptor objec
// lambda function that serves as the completion handler for the asynchronous operation. It will be called when a connection is accepted or when an error occurs during the acceptance process. The error parameter contains information about any errors that may have occurred.

        _acceptor.async_accept(*_socket, [this](const boost::system::error_code& error){
        // line creates a Tcp_Connection object using a Create function
        // This implies that the Tcp_Connection::Create function is designed to take ownership of the socket.
            auto connection = Tcp_Connection::Create(std::move(*_socket)); // clear the optinal
            // OnJoin is a callback or function object that gets executed when a connection is successfully established.
            // when a person has joined
            if(OnJoin){
                OnJoin(connection);
            }
    //     std::move(*_socket): This moves the socket from the std::optional into the Tcp_Connection object. This means that ownership of the socket is transferred to the Tcp_Connection object. After this line, the socket inside the std::optional will be in an empty state.
            //    The Tcp_Connection::Create function (presumably) constructs a Tcp_Connection object with the given socket and returns it.
            // line inserts a Tcp_Connection::pointer (which is a std::shared_ptr<Tcp_Connection>) into the connections unordered_set.

// connection is assumed to be a valid std::shared_ptr<Tcp_Connection> representing a connection that you want to add to the set of connections.

// By using an std::unordered_set, you can keep track of multiple connections in a way that ensures uniqueness. Each connection is represented by its shared pointer (std::shared_ptr), which allows multiple parts of your code to access and manage the connection while automatically handling memory management.
            connections.insert(connection);
            if (!error)
                //  If the connection was accepted without errors, the start() member function of the Tcp_Connection object associated with this new connection is called. This typically starts the communication process for this newly accepted client connection, which includes initiating asynchronous read/write operations as previously discussed.
                connection->start(
                        [this](const std::string &message) { if (OnClientMessage) { OnClientMessage(message); }},
                        [&, weak = std::weak_ptr(connection)] {
                            if (auto shared = weak.lock(); shared && connections.erase(shared)) {
                                if (OnLeave) OnLeave(shared);
                            }
                        }
                );

            // regardless of whether the connection was accepted successfully or not, startAccept() is called again at the end of the lambda function. This recursive call ensures that the server continues to listen for and accept new incoming connections, effectively keeping the server running and ready to accept new clients.
            startAccept();
        });
    }

}

