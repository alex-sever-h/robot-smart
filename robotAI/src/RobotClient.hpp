/*
 * RobotClient.hpp
 *
 *  Created on: Jun 14, 2013
 *      Author: alseh
 */

#ifndef ROBOTCLIENT_HPP_
#define ROBOTCLIENT_HPP_

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <iostream>
#include <string>
#include "RobotClientData.hpp"

using namespace std;

class RobotClient{

	boost::asio::io_service ioService;

	boost::asio::ip::tcp::resolver resolver;
	boost::asio::ip::tcp::socket sock;
	boost::array<char, 1024*256> readBuffer;
	boost::array<char, 4096> writeBuffer;

	RobotClientData *rcd;

	void write_handler(const boost::system::error_code &ec, std::size_t bytes_transferred);
	void readHeader(const boost::system::error_code &ec, std::size_t bytes_transferred);
	void readPayload(const boost::system::error_code &ec, std::size_t bytes_transferred);
	void connectHandler(const boost::system::error_code &ec);
	void resolve_handler(const boost::system::error_code &ec, boost::asio::ip::tcp::resolver::iterator it);

public:
	RobotClient();

	void stop(void);

	void sendString(string buffer);

	void setRcd(RobotClientData* rcd)
	{
		this->rcd = rcd;
	}
};


#endif /* ROBOTCLIENT_HPP_ */
