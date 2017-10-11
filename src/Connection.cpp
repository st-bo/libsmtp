
#include <array>
#include <string>

#include <boost/bind.hpp>

#include "Defs.h"
#include "Network/Smtp/Connection.h"


namespace Network {
	
	namespace Smtp {
		
		SmtpResolveException::SmtpResolveException(const boost::system::error_code& ec) {
			_ec = ec;
		}

		SmtpConnectException::SmtpConnectException(const boost::system::error_code& ec) {
			_ec = ec;
		}

		SmtpSendException::SmtpSendException(const boost::system::error_code& ec) {
			_ec = ec;
		}

		SmtpRecieveException::SmtpRecieveException(const boost::system::error_code& ec) {
			_ec = ec;
		}

		SmtpConnection::SmtpConnection(const std::string& host, 
			const std::string& port)
			: _socket(_service), _timer(_service)
		{
			_host = host;
			_port = port;
			//_timeout = boost::posix_time::milliseconds(timeout);
			
			_timer.expires_at(boost::posix_time::pos_infin);
			checkTimer();
		}

		const tcp::resolver::iterator SmtpConnection::resolve(
			const boost::posix_time::time_duration& timeout) {
			_timer.expires_from_now(timeout);

			tcp::resolver resolver(_service);
			auto q = tcp::resolver::query(_host, _port);
			return resolver.resolve(q);
		}

		void SmtpConnection::connect(int timeout) {
			auto t = boost::posix_time::milliseconds(timeout);
			auto endpoint = resolve(t);

			_timer.expires_from_now(t);
			
			boost::system::error_code ec = boost::asio::error::would_block;
			boost::asio::async_connect(_socket, 
				endpoint, 
				boost::bind(&SmtpConnection::handleErrorCode, _1, &ec));
			
			blockAsyncOperation(ec);
			
			if (ec || !_socket.is_open()) {
				throw SmtpConnectException(
					ec ? ec : boost::asio::error::operation_aborted);
			}
		}

		void SmtpConnection::disconnect() {
			_socket.close();
		}

		void SmtpConnection::send(int timeout, const std::string& payload) {
			_timer.expires_from_now(boost::posix_time::milliseconds(timeout));
			
			boost::system::error_code ec = boost::asio::error::would_block;
			boost::asio::async_write(_socket, boost::asio::buffer(payload), 
				boost::bind(&SmtpConnection::handleErrorCode, _1, &ec));
			blockAsyncOperation(ec);
			if (ec || !_socket.is_open()) {
				throw SmtpSendException(
					ec ? ec : boost::asio::error::operation_aborted);
			}
		}

		const std::string SmtpConnection::recieve(int timeout) {
			_timer.expires_from_now(boost::posix_time::milliseconds(timeout));

			std::string bytes;
			size_t len;
			for (;;) {
				std::array<char, 256> buffer;
				boost::system::error_code ec = boost::asio::error::would_block;
				_socket.async_read_some(boost::asio::buffer(buffer), 
					boost::bind(&SmtpConnection::handleRecieve, _1, _2, &ec, &len));
				blockAsyncOperation(ec);

				if (ec || !_socket.is_open()) {
					throw SmtpRecieveException(
						ec ? ec : boost::asio::error::operation_aborted);
				}
				
				bytes.append(buffer.data(), len);

				if (len < 256) {
					break;
				}
			}
			return bytes;
		}

		const std::string SmtpConnection::hostname() const {
			return boost::asio::ip::host_name();
		}

		/**
		* static function
		*/
		void SmtpConnection::handleErrorCode(const boost::system::error_code& ec, 
			boost::system::error_code* out_ec) {
			*out_ec = ec;
		}

		/**
		* static function
		*/
		void SmtpConnection::handleRecieve(const boost::system::error_code& ec, 
			std::size_t length,
			boost::system::error_code* out_ec, 
			std::size_t* out_length) {
			*out_ec = ec;
			*out_length = length;
		}

		void SmtpConnection::blockAsyncOperation(boost::system::error_code& ec) {
			ec = boost::asio::error::would_block;
			do _service.run_one(); while (ec == boost::asio::error::would_block);
		}

		void SmtpConnection::checkTimer() {
			if (_timer.expires_at() <= boost::asio::deadline_timer::traits_type::now()) {
				disconnect();
				_timer.expires_at(boost::posix_time::pos_infin);
				throw SmtpTimeoutException();
			}
			_timer.async_wait(boost::bind(&SmtpConnection::checkTimer, this));
		}


	} // NS Smtp

} // NS Network