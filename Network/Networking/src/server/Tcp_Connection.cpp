//
// Created by rajat on 9/22/23.
//
#include <Network/server/Tcp_Connection.h>
#include<iostream>

namespace MOVF {

    Tcp_Connection::Tcp_Connection(tcp::socket&& _socket) : socket(std::move (_socket)){
        boost::system::error_code ec;

        std::stringstream name;
        name << socket.remote_endpoint();

        username = name.str();
    }

    void Tcp_Connection::start(MessageHandler&& messageHandler, ErrorHandler&& errorHandler){
        _messageHandler = std::move(messageHandler);
        _errorHandler = std::move(errorHandler);
        asyncRead();
    }

    void Tcp_Connection::asyncRead(){
        boost::asio::async_read_until(socket, streambuf , "\n" , [self = shared_from_this()] (boost::system::error_code er,size_t bytes_transfered){
           self->onRead(er,bytes_transfered);
        });
    }

    void Tcp_Connection::onRead(boost::system::error_code er , size_t bytes_transfered){
        if(er){
            socket.close(er);
            // socket.close(er) ; // if error found here and wanted to show that error
            _errorHandler;
            return;
        }
        std::stringstream message;
        message << username << ":" << std::istream(&streambuf).rdbuf();
        streambuf.consume(bytes_transfered);
        _messageHandler(message.str());
        // add a message handler
        asyncRead();
    }
    void Tcp_Connection::Post(const std::string &message){
        bool queueIdle = outgoingMessage.empty();
        outgoingMessage.push(message);
        if(queueIdle){
            asyncWrite();
        }
    }
    void Tcp_Connection::asyncWrite(){
        boost::asio::async_write(socket, boost::asio::buffer(outgoingMessage.front()) ,  [self = shared_from_this()] (boost::system::error_code er,size_t bytes_transfered){
            self->onWrite(er,bytes_transfered);
        });
    }
    void Tcp_Connection::onWrite(boost::system::error_code er , size_t bytes_transfered){
        if(er){
            socket.close(er);
            // socket.close(er) ; // if error found here and wanted to show that error
            _errorHandler;
            return;
        }
        outgoingMessage.pop();
        if(!outgoingMessage.empty()){
            asyncWrite();
        }
    }
}
