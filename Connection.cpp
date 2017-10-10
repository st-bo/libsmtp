
#include <array>
#include <string>

#include "Defs.h"
#include "Network/Smtp/Connection.h"


namespace Network {
	
	namespace Smtp {
		
		namespace Connection {

			SmtpSendException::SmtpSendException(const boost::system::error_code& ec) {
				_ec = ec;
			}

			SmtpConnection::SmtpConnection(const std::string& host, const std::string& port, int timeout)
				: _socket(_service)
			{
				_host = host;
				_port = port;
				_timeout = timeout;
			}

			void SmtpConnection::connect() {
				tcp::resolver resolver(_service);
				tcp::resolver::iterator endpoint = resolver.resolve(tcp::resolver::query(_host, _port));

				_socket = tcp::socket(_service);

				boost::asio::connect(_socket, endpoint);
			}
			void SmtpConnection::disconnect() {
				_socket.close();
			}

			void SmtpConnection::send(const std::string& payload) {
				boost::asio::write(_socket, boost::asio::buffer(payload));
			}

			const std::string SmtpConnection::recieve() {
				std::string bytes;
				std::array<uint8_t, 256> buffer;
				boost::system::error_code ec;
				size_t len;
				for (;;) {
					len = _socket.read_some(boost::asio::buffer(buffer), ec);
					if (ec) {
						throw SmtpSendException(ec);
					}
				}
				return bytes;
			}

			const std::string SmtpConnection::hostname() const {
				return boost::asio::ip::host_name();
			}

		} // NS Connection
	} // NS Smtp
} // NS Network