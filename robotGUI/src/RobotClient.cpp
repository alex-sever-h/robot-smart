/*
 * RobotClient.cpp
 *
 *  Created on: Jun 14, 2013
 *      Author: alseh
 */

#include "RobotClient.hpp"
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include "protobuf/robotdata.pb.h"

void RobotClient::readHeader(const boost::system::error_code& ec,
		std::size_t bytes_transferred)
{
	if (!ec)
	{
		cout << "rheader: " << packHeader[0] << " ** " << packHeader[1] << endl;

		if(bytes_transferred == 8)
		{
			//read the payload
			boost::asio::async_read(*sock, boost::asio::buffer(readBuffer, packHeader[1]),
					boost::bind(&RobotClient::readPayload, this,
							boost::asio::placeholders::error,
							boost::asio::placeholders::bytes_transferred));
		}
		else
			cout << "r: " << bytes_transferred << " oof: " << 8 << endl;

		// do some more reading
	}
	else
	{
		cout << "read err: " << ec << endl;

		boost::asio::ip::tcp::resolver::query query(CommSettings::getHostname(), CommSettings::getPortString());
		resolver.async_resolve(query,
				boost::bind(&RobotClient::addrResolvedConnectHandler, this,
						boost::asio::placeholders::error(),
						boost::asio::placeholders::iterator()));
	}
}


void RobotClient::readPayload(const boost::system::error_code& ec,
		std::size_t bytes_transferred)
{
	if (!ec)
	{
		if(bytes_transferred == packHeader[1])
		{
			cout << "payload read: " << bytes_transferred << endl;
			//process payload data
			if(rcd)
				rcd->updateFromBuffer(readBuffer.c_array(), packHeader[1], packHeader[0]);

			//read next packet
			boost::asio::async_read(*sock, boost::asio::buffer(packHeader, 8),
					boost::bind(&RobotClient::readHeader, this,
							boost::asio::placeholders::error,
							boost::asio::placeholders::bytes_transferred));
		}
		else
			cout << "r: " << bytes_transferred << " oof: " << packHeader[1] << endl;
	}
	else
	{
		cout << "read err: " << ec << endl;

		boost::asio::ip::tcp::resolver::query query(CommSettings::getHostname(), CommSettings::getPortString());
		resolver.async_resolve(query,
				boost::bind(&RobotClient::addrResolvedConnectHandler, this,
						boost::asio::placeholders::error(),
						boost::asio::placeholders::iterator()));
	}
}

void RobotClient::connectHandler(const boost::system::error_code& ec)
{
	if (!ec)
	{
		cout << "clientConnected ......" << endl;
		//start listening
		boost::asio::async_read(*sock, boost::asio::buffer(packHeader, 8),
				boost::bind(&RobotClient::readHeader, this,
						boost::asio::placeholders::error(),
						boost::asio::placeholders::bytes_transferred()));
	}
	else
	{
		cout << "connect failed ...." << endl;
		cout << "read err: " << ec << endl;

		boost::asio::ip::tcp::resolver::query query(CommSettings::getHostname(), CommSettings::getPortString());
		resolver.async_resolve(query,
				boost::bind(&RobotClient::addrResolvedConnectHandler, this,
						boost::asio::placeholders::error(),
						boost::asio::placeholders::iterator()));
	}
}

void RobotClient::addrResolvedConnectHandler(const boost::system::error_code& ec,
		boost::asio::ip::tcp::resolver::iterator it)
{
	if (!ec)
	{
		// Create new socket (old one is destroyed automatically)
		if(sock)
			delete sock;
		sock = new boost::asio::ip::tcp::socket(ioService);

		sleep(1);

		sock->async_connect(*it, boost::bind(&RobotClient::connectHandler,
				this, boost::asio::placeholders::error()));
	}
	else
	{
		cout << "resolve failed ...." << endl;
		cout << "read err: " << ec << endl;
	}
}

void RobotClient::writeMessageHandler(void *data,
		const boost::system::error_code& ec,
		std::size_t bytes_transferred)
{
	//writing completed .... deleting loose ends
	if(data != NULL)
		delete data;

	if(!sendQueue.empty())
	{
		sendQueueLock.lock();
		void * dataBuffer = sendQueue.front();
		size_t  dataSize  = sendQueueSize.front();
		sendQueue.pop_front();
		sendQueueSize.pop_front();
		sendQueueLock.unlock();


		boost::asio::async_write(*sock, boost::asio::buffer(dataBuffer, dataSize),
				boost::bind(&RobotClient::writeMessageHandler, this,
						dataBuffer,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred));
	}
	else
	{
		sendingInProgress.unlock();
	}
}

RobotClient::RobotClient() : resolver(ioService)
{
	sock = NULL;

	boost::asio::ip::tcp::resolver::query query(CommSettings::getHostname(), CommSettings::getPortString());
	resolver.async_resolve(query,
			boost::bind(&RobotClient::addrResolvedConnectHandler, this,
					boost::asio::placeholders::error(),
					boost::asio::placeholders::iterator()));

	//prepare buffers for reading
	headerBuffer.push_back( boost::asio::buffer(&enumDataType, sizeof(enumDataType) ) );
	headerBuffer.push_back( boost::asio::buffer(&nData, sizeof(nData) ) );

	boost::thread io_service_thread(boost::bind(&boost::asio::io_service::run, &ioService));

	cout << "--------------client initialized---------------------\n";
}

void RobotClient::stop(void)
{
	ioService.stop();
}

void RobotClient::sendString(string buffer)
{
//	boost::asio::async_write(*sock, boost::asio::buffer(buffer),
//			boost::bind(&RobotClient::write_handler, this,
//					boost::asio::placeholders::error(),
//					boost::asio::placeholders::bytes_transferred()));
}


void RobotClient::sendSerializedData(google::protobuf::Message *data,
		enum dataType enumDataType)
{
	size_t sizeHeader = sizeof(enumDataType) + sizeof(uint32_t);
	uint32_t sizePayload = data->ByteSize();
	uint8_t *byteData = new uint8_t[sizeHeader + sizePayload];


	((uint32_t *)byteData)[0] = enumDataType;
	((uint32_t *)byteData)[1] = sizePayload;

	data->SerializeToArray(byteData + sizeHeader, sizePayload);

	sendQueueLock.lock();
	sendQueue.push_back(byteData);
	sendQueueSize.push_back(sizeHeader + sizePayload);
	sendQueueLock.unlock();

	if(sendingInProgress.try_lock())
	{
		boost::system::error_code ec;
		writeMessageHandler(NULL, ec, 0);
	}

}
