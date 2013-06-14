/*
 * RobotClient.cpp
 *
 *  Created on: Jun 14, 2013
 *      Author: alseh
 */

#include "RobotClient.hpp"
#include <boost/thread.hpp>
#include <iostream>

void RobotClient::read_handler(const boost::system::error_code& ec,
		std::size_t bytes_transferred)
{
	if (!ec)
	{
		std::cout << std::string(buffer.data(), bytes_transferred) << std::endl;
		sock.async_read_some(boost::asio::buffer(buffer),
				boost::bind(&RobotClient::read_handler, this, boost::asio::placeholders::error(), boost::asio::placeholders::bytes_transferred()));
	}
	std::cout << "read\n";
}

void RobotClient::connect_handler(const boost::system::error_code& ec)
{
	if (!ec)
	{
		boost::asio::write(sock, boost::asio::buffer("GET / HTTP 1.1\r\nHost: highscore.de\r\n\r\n"));
		sock.async_read_some(boost::asio::buffer(buffer),
				boost::bind(&RobotClient::read_handler, this, boost::asio::placeholders::error(), boost::asio::placeholders::bytes_transferred()));
	}
}

void RobotClient::resolve_handler(const boost::system::error_code& ec,
		boost::asio::ip::tcp::resolver::iterator it)
{
	if (!ec)
	{
		sock.async_connect(*it, boost::bind(&RobotClient::connect_handler, this, boost::asio::placeholders::error()));
	}
}

RobotClient::RobotClient() :
							resolver(io_service),
							sock(io_service)
{
	boost::asio::ip::tcp::resolver::query query("localhost", "50000");
	resolver.async_resolve(query,
			boost::bind(&RobotClient::resolve_handler, this, boost::asio::placeholders::error(), boost::asio::placeholders::iterator()));
	io_service.run();
}


