/*
 * RobotServer.c
 *
 *  Created on: Jun 14, 2013
 *      Author: alseh
 */

#include "RobotServer.hpp"
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>

void RobotServer::write_handler(const boost::system::error_code& ec,
		std::size_t bytes_transferred)
{
}

void RobotServer::accept_handler(const boost::system::error_code& ec)
{
	if (!ec)
	{
		boost::asio::async_write(sock, boost::asio::buffer(buffer), boost::bind(&RobotServer::write_handler, this, boost::asio::placeholders::error(), boost::asio::placeholders::bytes_transferred()));
		//buffer.clear();
	}
}

RobotServer::RobotServer()
: endpoint(boost::asio::ip::tcp::v4(), ROBOSERVERPORT),
  acceptor(io_service, endpoint),
  sock(io_service)
{
	buffer = "Hello, world! from server\n";
	acceptor.listen();
	acceptor.async_accept(sock, boost::bind(&RobotServer::accept_handler, this, boost::asio::placeholders::error()));
	io_service.run();
}

int i = 0;
void RobotServer::addString(string blabla)
{
	//buffer.append(blabla);
	blabla.append(boost::lexical_cast<string>(i));
	boost::asio::async_write(sock, boost::asio::buffer(blabla), boost::bind(&RobotServer::write_handler, this, boost::asio::placeholders::error(), boost::asio::placeholders::bytes_transferred()));
	i++;
}
