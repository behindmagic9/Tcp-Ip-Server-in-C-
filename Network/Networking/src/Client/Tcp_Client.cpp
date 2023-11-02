#include <Network/Client/Tcp_Client.h>

namespace MOVF{
    TcpCLient::TcpCLient(const std::string &address, int port) : socket{ioContext} {
        boost::asio:: ip::tcp::resolver resolver{ioContext};
        endpoints = resolver.resolve(address,std::to_string(port));
    }
    void TcpCLient::Run() {
        boost::asio::async_connect(socket,endpoints,[this](boost::system::error_code ec, boost::asio::ip::tcp::endpoint ep){
            if(!ec){
                asyncRead();
            }
        });
        ioContext.run();
    }
    void TcpCLient::Post(const std::string &message) {
        bool queueIdle = outgoingMessage.empty();
        outgoingMessage.push(message);
        if(queueIdle){
            asyncWrite();
        }
    }
    void TcpCLient::Stop() {
        boost::system::error_code ec;
        socket.close();
        if(ec){
            // process the error
        }
    }

    void TcpCLient::asyncRead() {
        boost::asio::async_read_until(socket,streambuf,"\n",[this](boost::system::error_code ec , size_t bytes_transfered){
            onRead(ec, bytes_transfered);
        });
    }
    void TcpCLient::onRead(boost::system::error_code ec, std::size_t bytes_transfered) {
        if(ec){
            Stop();
            return;
        }
        std::stringstream message;
        message << std::istream{&streambuf}.rdbuf();
        OnMessage(message.str());
        asyncRead();
    }

    void TcpCLient::asyncWrite() {
        boost::asio::async_write(socket,boost::asio::buffer(outgoingMessage.front()),[this](boost::system::error_code ec , size_t bytes_transfered){
            onWrite(ec, bytes_transfered);
        });
    }
    void TcpCLient::onWrite(boost::system::error_code ec, std::size_t bytes_tranfered) {
        if(ec){
            Stop();
            return;
        }
        outgoingMessage.pop();
        if(!outgoingMessage.empty()){
            asyncWrite();
        }
    }

}