/*
 * RobotServer.c
 *
 *  Created on: Jun 14, 2013
 *      Author: alseh
 */

#include "RobotServer.hpp"
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/asio.hpp>
#include "Location.hpp"


void RobotServer::readHeader(const boost::system::error_code& ec,
		std::size_t bytes_transferred)
{
	if (!ec)
	{
		cout << "rheader: " << packHeader[0] << " ** " << packHeader[1] << endl;

		if(bytes_transferred == 8)
		{
			//read the payload
			boost::asio::async_read(sock, boost::asio::buffer(readBuffer, packHeader[1]),
					boost::bind(&RobotServer::readPayload, this,
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

		//		boost::asio::ip::tcp::resolver::query query(CommSettings::getHostname(), CommSettings::getPortString());
		//		resolver.async_resolve(query,
		//				boost::bind(&RobotServer::addrResolvedConnectHandler, this,
		//						boost::asio::placeholders::error(),
		//						boost::asio::placeholders::iterator()));
	}
}


void RobotServer::readPayload(const boost::system::error_code& ec,
		std::size_t bytes_transferred)
{
	if (!ec)
	{
		if(bytes_transferred == packHeader[1])
		{
			cout << "payload read: " << bytes_transferred << endl;
			//process payload data

			switch(packHeader[0])
			{
			case TARGET_POSITIONING:
			{
				robotdata::Target target;
				target.ParseFromArray(readBuffer.c_array(), packHeader[1]);
				physicalRobot->moveAtLocation(
						Location(target.posx(), target.posy(),target.theta()));
			}break;
			default:
				cout << "unknown package" << endl;
				break;
			}

			//read next packet
			boost::asio::async_read(sock, boost::asio::buffer(packHeader, 8),
					boost::bind(&RobotServer::readHeader, this,
							boost::asio::placeholders::error,
							boost::asio::placeholders::bytes_transferred));
		}
		else
			cout << "r: " << bytes_transferred << " oof: " << packHeader[1] << endl;
	}
	else
	{
		cout << "read err: " << ec << endl;

		//		boost::asio::ip::tcp::resolver::query query(CommSettings::getHostname(), CommSettings::getPortString());
		//		resolver.async_resolve(query,
		//				boost::bind(&RobotServer::addrResolvedConnectHandler, this,
		//						boost::asio::placeholders::error(),
		//						boost::asio::placeholders::iterator()));
	}
}

void RobotServer::accept_handler(const boost::system::error_code& ec)
{
	//std::cout << "***server accept handler called***\n";

	if (!ec)
	{
		//start listening
		boost::asio::async_read(sock, boost::asio::buffer(packHeader, 8),
				boost::bind(&RobotServer::readHeader, this,
						boost::asio::placeholders::error(),
						boost::asio::placeholders::bytes_transferred()));
	}
}

RobotServer::RobotServer(RobotModel *physicalRobot) : endpoint(boost::asio::ip::tcp::v4(), CommSettings::getPortInt()), acceptor(ioService, endpoint), sock(ioService)
{
	this->physicalRobot = physicalRobot;
	acceptor.listen();
	acceptor.async_accept(sock,
			boost::bind(&RobotServer::accept_handler, this,
					boost::asio::placeholders::error()));

	boost::thread io_service_thread(boost::bind(&boost::asio::io_service::run, &ioService));

}

void RobotServer::writeMessageHandler(void *data,
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


		boost::asio::async_write(sock, boost::asio::buffer(dataBuffer, dataSize),
				boost::bind(&RobotServer::writeMessageHandler, this,
						dataBuffer,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred));
	}
	else
	{
		sendingInProgress.unlock();
	}
}

void RobotServer::sendString(string blabla)
{
	//	boost::asio::async_write(sock, boost::asio::buffer(blabla),
	//			boost::bind(&RobotServer::writeMessageHandler, this,
	//					NULL,
	//					boost::asio::placeholders::error,
	//					boost::asio::placeholders::bytes_transferred));
}


void RobotServer::sendSerializedData(void* data, int nData,
		enum dataType enumDataType)
{
	std::vector<boost::asio::const_buffer> buffers;
	buffers.push_back( boost::asio::buffer(&enumDataType, sizeof(enumDataType) ) );
	buffers.push_back( boost::asio::buffer(&nData, sizeof(nData) ) );
	buffers.push_back( boost::asio::buffer(data, nData ) );

	boost::asio::async_write(sock, buffers,
			boost::bind(&RobotServer::writeMessageHandler, this,
					data,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));

}


void RobotServer::sendSerializedData(google::protobuf::Message *data,
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

void RobotServer::stop(void)
{
	ioService.stop();
}
