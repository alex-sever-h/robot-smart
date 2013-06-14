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

class RobotClient{

	boost::asio::io_service io_service;

	boost::asio::ip::tcp::resolver resolver;
	boost::asio::ip::tcp::socket sock;
	boost::array<char, 4096> buffer;

	void read_handler(const boost::system::error_code &ec, std::size_t bytes_transferred);
	void connect_handler(const boost::system::error_code &ec);
	void resolve_handler(const boost::system::error_code &ec, boost::asio::ip::tcp::resolver::iterator it);

public:
	RobotClient();

};


#endif /* ROBOTCLIENT_HPP_ */
