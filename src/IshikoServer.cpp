/*
    Copyright (c) 2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/nemu-cpp/ishiko-connection-engine/blob/main/LICENSE.txt
*/

#include "IshikoServer.hpp"
#include "IshikoWebRequest.hpp"
#include "IshikoWebResponseBuilder.hpp"
#include <Ishiko/HTTP.hpp>

using namespace Ishiko;
using namespace std;

namespace Nemu
{

IshikoServer::IshikoServer(IPv4Address address, Port port, Error& error)
    : m_init(error), m_socket(address, port, error)
{
}

void IshikoServer::start()
{
    // TODO: as a quick hack we put the blocking stuff in a secondary thread
    m_acceptThread = thread(
        [this]()
        {
            // TODO: this is a temporary blocking implementation
            // TODO: handle error
            Error error;
            TCPClientSocket clientSocket = m_socket.accept(error);
            
            IshikoWebRequest request;
            HTTPMessagePushParser requestParser(request);

            // TODO: how big should this buffer be? Adjust automatically?
            char buffer[1000];
            int n = clientSocket.read(buffer, 1000, error); // TODO: handle error
            while (!requestParser.onData(boost::string_view(buffer, n)) && (n != 0))
            {
                n = clientSocket.read(buffer, 1000, error);
            }
            if (n == 0)
            {
                // TODO: we received a partial request
            }
            else
            {
                const Route& route = m_routes->match(request.URI());

                Views views;
                IshikoWebResponseBuilder response(views);

                route.runHandler(request, response, *m_logger);


                
                string responseString = response.toString();
                clientSocket.write(responseString.c_str(), responseString.size(), error);
            }


            // TODO: loop and do something with the connected stuff
        }
    );
}

void IshikoServer::stop()
{
    // TODO: for now send a dummy request to trigger the accept
    Error error;
    TCPClientSocket socket(error);
    socket.connect(m_socket.ipAddress(), m_socket.port(), error);
}

void IshikoServer::join()
{
    m_acceptThread.join();
}

bool IshikoServer::isRunning() const
{
    // TODO
    return false;
}

const TCPServerSocket& IshikoServer::socket() const
{
    return m_socket;
}

}
