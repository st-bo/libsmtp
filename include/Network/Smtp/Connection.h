#pragma once

#include <string>

#include <boost/asio.hpp>

using boost::asio::ip::tcp;

namespace Network {
	
	namespace Smtp {

		class SmtpResolveException : public std::exception {
		public:
			SmtpResolveException(const boost::system::error_code& ec);

		private:
			boost::system::error_code _ec;
		};

		class SmtpConnectException : public std::exception {
		public:
			SmtpConnectException(const boost::system::error_code& ec);

		private:
			boost::system::error_code _ec;
		};

		class SmtpDisconnectException : public std::exception {};

		class SmtpTimeoutException : public std::exception {};

		class SmtpSendException : public std::exception {
		public:
			SmtpSendException(const boost::system::error_code& ec);

		private:
			boost::system::error_code _ec;
		};

		class SmtpRecieveException : public std::exception {
		public:
			SmtpRecieveException(const boost::system::error_code& ec);

		private:
			boost::system::error_code _ec;
		};

		class SmtpDialogException : public std::exception {};

		class SmtpConnection {
		public:
			SmtpConnection(const std::string& host, const std::string& port);

			void connect(int timeout);
			void disconnect();
			void send(int timeout, const std::string& payload);
			const std::string recieve(int timeout);
				
			const std::string hostname() const;
		private:
			std::string _host;
			std::string _port;
			boost::asio::io_service _service;
			boost::asio::deadline_timer _timer;
			tcp::socket _socket;
			
			const tcp::resolver::iterator resolve(const boost::posix_time::time_duration& timeout);

			static void handleErrorCode(const boost::system::error_code& ec, 
				boost::system::error_code* out_ec);
			static void handleRecieve(const boost::system::error_code& ec, 
				std::size_t length, 
				boost::system::error_code* out_ec, 
				std::size_t* out_length);
			
			void blockAsyncOperation(boost::system::error_code& ec);
			void checkTimer();
		};

	} // NS Smtp

} // NS Network