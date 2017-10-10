
#include <time.h>

#include <string>
#include <sstream>
#include <vector>

#include "Defs.h"
#include "Network/Smtp/Client.h"
#include "Network/Smtp/Command.h"
#include "Network/Smtp/Connection.h"

namespace Network {
	
	namespace Smtp {

		namespace Client {

			ParseResponseException::ParseResponseException(const std::string& payload) {
				_payload = payload;
			}

			static const Response parseResponse(const std::string& payload) {
				int status;
				std::string bytes;
				int ival = sscanf(payload.c_str(), "%d %s\r\n", &status, &bytes[0]);
				if (ival != 2) {
					throw ParseResponseException(payload);
				}

				Response res;
				res.status = static_cast<Smtp::Status>(status);
				res.bytes = bytes;
				return res;
			}

			class SmtpClientImpl {
			public:
				SmtpClientImpl(const std::string& host, const std::string& port, int timeout);

				void sendMail(const SmtpAddress from,
					const std::vector<SmtpAddress>& vec_to,
					const std::string& subject,
					const std::string& body) const;

			private:
				Connection::SmtpConnection* _conn;

				const Response communicate(Command::SmtpCommand cmd) const;

			};


			SmtpClient::SmtpClient(const std::string& host, const std::string& port, int timeout) {
				_impl = new SmtpClientImpl(host, port, timeout);
			}

			void SmtpClient::sendMail(const SmtpAddress from, const std::vector<SmtpAddress>& vec_to, const std::string& subject, const std::string& body) const {
				if (_impl) {
					_impl->sendMail(from, vec_to, subject, body);
				}
			}
			
			SmtpClientImpl::SmtpClientImpl(const std::string& host, const std::string& port, int timeout) {
				_conn = new Connection::SmtpConnection(host, port, timeout);
			}

			void SmtpClientImpl::sendMail(const SmtpAddress from, const std::vector<SmtpAddress>& vec_to, const std::string& subject, const std::string& body) const {
				
				// build a Mime Mail Message (bytes) - Mime Header list + plain text body
				// current date
				// from
				// to
				// subject
				// date
				// content-type
				// CRLF
				// body
				
				// run simple SMTP workflow
				// HELO FOO.com
				// MAIL FROM: <sender@example.com>
				// RCPT TO: <recipient@example.com>
				// DATA
				// ... bytes ...
				// CRLF "." CRLF
				// QUIT

				// create headers
				std::ostringstream mail;
				mail << "From: " << from.toString() << CRLF;
				for (auto to : vec_to) {
					mail << "To: " << to.toString() << CRLF;
				}
				mail << "Subject: " << subject << CRLF;
				mail << "Date: " << time(0) << CRLF;
				mail << "Content-Type: text/plain" << CRLF;
				mail << CRLF;
				mail << body;

				// socket setup
				_conn->connect();

				// smtp dialog
				communicate(Command::HeloCommand(_conn->hostname()));
				communicate(Command::MailFromCommand(from.getEmail()));
				for (auto to : vec_to) {
					communicate(Command::RecipientToCommand(to.getEmail()));
				}
				communicate(Command::DataCommand(mail.str()));
				communicate(Command::DataFinishedCommand());
				communicate(Command::QuitCommand());
				
				// socket shutdown
				_conn->disconnect();
			}
			
			const Response SmtpClientImpl::communicate(Command::SmtpCommand cmd) const {
				// send command
				// recieve response bytes (payload)
				// return parsed response
				_conn->send(cmd.toString());
				return parseResponse(_conn->recieve());
			}			

		} // NS Client

	} // NS Smtp

} // NS Network