
#include <time.h>

#include <string>
#include <sstream>
#include <vector>

#include "Defs.h"

#include "Network/Smtp/Client.h"
#include "Network/Smtp/Command.h"
#include "Network/Smtp/Connection.h"
#include "Network/Smtp/Response.h"

namespace Network {
	
	namespace Smtp {
		
		class SmtpClientImpl {
		public:
			SmtpClientImpl(const std::string& host,
				const std::string& port, 
				int connectTimeout, 
				int sendTimeout, 
				int recieveTimeout);

			void sendMail(const SmtpAddress from,
				const std::vector<SmtpAddress>& vec_to,
				const std::string& subject,
				const std::string& body) const;

		private:
			SmtpConnection* _conn;
			int _connectTimeout;
			int	_sendTimeout;
			int _recieveTimeout;

			const Response communicate(SmtpCommand cmd) const;

		};


		SmtpClient::SmtpClient(const std::string& host,
			const std::string& port,
			int connTimeout, 
			int sndTimeout, 
			int rcvTimeout) {
			_impl = new SmtpClientImpl(host, port, connTimeout, sndTimeout, rcvTimeout);
		}

		void SmtpClient::sendMail(const SmtpAddress from, 
			const std::vector<SmtpAddress>& vec_to, 
			const std::string& subject, 
			const std::string& body) const {
			if (_impl) {
				_impl->sendMail(from, vec_to, subject, body);
			}
		}
			
		SmtpClientImpl::SmtpClientImpl(const std::string& host, 
			const std::string& port, 
			int connTimeout, 
			int sndTimeout, 
			int rcvTimeout) {
			_connectTimeout = connTimeout;
			_sendTimeout = sndTimeout;
			_recieveTimeout = rcvTimeout;
			_conn = new SmtpConnection(host, port);
		}

		void SmtpClientImpl::sendMail(const SmtpAddress from, 
			const std::vector<SmtpAddress>& vec_to, 
			const std::string& subject, 
			const std::string& body) const {
				
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
			_conn->connect(_connectTimeout);

			// smtp dialog
			communicate(HeloCommand(_conn->hostname()));
			communicate(MailFromCommand(from.getEmail()));
			for (auto to : vec_to) {
				communicate(RecipientToCommand(to.getEmail()));
			}
			communicate(DataCommand(mail.str()));
			communicate(DataFinishedCommand());
			communicate(QuitCommand());
				
			// socket shutdown
			_conn->disconnect();
		}
			
		const Response SmtpClientImpl::communicate(SmtpCommand cmd) const {
			// send command
			// recieve response bytes (payload)
			// return parsed response
			_conn->send(_sendTimeout, cmd.toString());
			return parseResponse(_conn->recieve(_recieveTimeout));
		}			

	} // NS Smtp

} // NS Network