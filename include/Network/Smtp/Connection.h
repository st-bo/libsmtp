#pragma once

#include <string>

#include <boost/asio.hpp>

using boost::asio::ip::tcp;

namespace Network {
	
	namespace Smtp {

		class SmtpConnectException : public std::exception {};

		class SmtpDisconnectException : public std::exception {};

		class SmtpTimeoutException : public std::exception {};

		class SmtpSendException : public std::exception {
		public:
			SmtpSendException(const boost::system::error_code& ec);

		private:
			boost::system::error_code _ec;
		};

		class SmtpRecieveException : public std::exception {};

		class SmtpDialogException : public std::exception {};

		class SmtpConnection {
		public:
			SmtpConnection(const std::string& host, const std::string& port, int timeout);

			void connect();
			void disconnect();
			void send(const std::string& payload);
			const std::string recieve();
				
			const std::string hostname() const;
		private:
			std::string _host;
			std::string _port;
			int _timeout;
			boost::asio::io_service _service;
			tcp::socket _socket;
			tcp::resolver::iterator _endpoint;
		};

	} // NS Smtp

} // NS Network