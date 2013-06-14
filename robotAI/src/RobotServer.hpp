/*
 * RobotServer.h
 *
 *  Created on: Jun 14, 2013
 *      Author: alseh
 */

#ifndef ROBOTSERVER_H_
#define ROBOTSERVER_H_

#include <boost/asio.hpp>
#include <string>

#define ROBOSERVERPORT 50000

using namespace std;


class RobotServer{
	boost::asio::io_service io_service;
	boost::asio::ip::tcp::endpoint endpoint;
	boost::asio::ip::tcp::acceptor acceptor;
	boost::asio::ip::tcp::socket sock;
	std::string buffer;

	void write_handler(const boost::system::error_code &ec, std::size_t bytes_transferred);
	void accept_handler(const boost::system::error_code &ec);

public:
	RobotServer();

	void addString(string blabla);

};


#endif /* ROBOTSERVER_H_ */
