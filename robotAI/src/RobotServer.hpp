/*
 * RobotServer.h
 *
 *  Created on: Jun 14, 2013
 *      Author: alseh
 */

#ifndef ROBOTSERVER_H_
#define ROBOTSERVER_H_

#include "protobuf/robotdata.pb.h"
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <string>
#include <list>
#include "CommunicationShared.hpp"
#include "robotModel.hpp"

using namespace std;

class RobotModel;

class RobotServer{
	RobotModel *physicalRobot;

	boost::asio::io_service ioService;
	boost::asio::ip::tcp::endpoint endpoint;
	boost::asio::ip::tcp::acceptor acceptor;
	boost::asio::ip::tcp::socket sock;
	boost::array<char, 1024> writeBuffer;
	boost::array<char, 1024> readBuffer;

	list<void *> sendQueue;
	list<size_t> sendQueueSize;
	boost::mutex sendQueueLock;

	boost::mutex sendingInProgress;


	int enumDataType;
	size_t nData;

	uint32_t packHeader[2];

	std::vector<boost::asio::mutable_buffer> headerBuffer;

	void read_handler(const boost::system::error_code &ec, std::size_t bytes_transferred);
	void writeMessageHandler(void *data, const boost::system::error_code &ec, std::size_t bytes_transferred);
	void accept_handler(const boost::system::error_code &ec);

	void readHeader(const boost::system::error_code& ec,
			std::size_t bytes_transferred);
	void readPayload(const boost::system::error_code& ec,
			std::size_t bytes_transferred);


public:
	RobotServer(RobotModel *physicalRobot);

	void sendString(string blabla);
	void sendSerializedData(void * data, int nData, enum dataType);
	void sendSerializedData(google::protobuf::Message * data, enum dataType);

	void stop(void);

};


#endif /* ROBOTSERVER_H_ */
