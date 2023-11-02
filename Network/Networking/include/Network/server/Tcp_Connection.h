//
// Created by rajat on 9/22/23.
//

#ifndef TCP_CONNECTION_H
#define TCP_CONNECTION_H

#include<memory>
#include<queue>
#include <boost/asio.hpp>

namespace MOVF{
    using boost::asio::ip::tcp;

    using MessageHandler = std::function<void(std::string)>;
    using ErrorHandler = std::function<void()>;

    class Tcp_Connection :public std::enable_shared_from_this<Tcp_Connection>{
    public:
        using pointer = std::shared_ptr<Tcp_Connection>;
        /*
             boost::asio::io_context& ioContext is a reference to an io_context object that will be passed as a parameter when creating a new Tcp_Conention instance.

    return pointer(new Tcp_Conention(ioContext));: This line creates a new instance of the Tcp_Conention class, passing the ioContext as a parameter to its constructor. It then wraps the newly created object in a std::shared_ptr and returns that shared pointer.

    By returning a std::shared_ptr, the factory function ensures that the ownership of the Tcp_Conention object is shared among multiple parts of your program. As long as there are shared pointers referencing the object, it won't be deleted. This is useful for managing the object's lifetime and preventing premature destruction.

         */
        //  factory function Create for creating instances of a class called Tcp_Conention using std::shared_ptr.
        static pointer Create(tcp::socket&& socket){
            return pointer(new Tcp_Connection(std::move (socket) ));
        }
        tcp::socket& Socket(){
            return socket;
        }
        inline const std::string& GetUserName() const {
            return username;
        }
        void start(MessageHandler&& messageHandler, ErrorHandler&& errorHandler);
        void Post(const std::string &message);
    private:
        explicit Tcp_Connection(tcp::socket&& socket);
        void asyncRead();
        void onRead(boost::system::error_code er , size_t bytes_transfered);
        void asyncWrite();
        void onWrite(boost::system::error_code er , size_t bytes_transfered);

    private:
        tcp::socket socket;
        std::string username;
        std::queue<std::string> outgoingMessage;
        boost::asio::streambuf streambuf{65536};
        MessageHandler _messageHandler;
        ErrorHandler _errorHandler;
    };
}
#endif //NETWORK_TCP_CONNECTION_H
