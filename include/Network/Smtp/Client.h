#pragma once

#include <string>
#include <vector>

#include "Network/Smtp.h"


namespace Network {
	
	namespace Smtp {
				
		namespace Client {

			class ParseResponseException : std::exception {
			public:
				ParseResponseException(const std::string& payload);
			private:
				std::string _payload;
			};


			class SmtpClientImpl;

			class SmtpClient {
			public:
				SmtpClient(const std::string& host, const std::string& port, int timeout);

				void sendMail(const SmtpAddress from, 
					const std::vector<SmtpAddress>& vec_to, 
					const std::string& subject, 
					const std::string& body) const;

			private:
				SmtpClientImpl* _impl;
				
			};

		} // NS Client
	} // NS Smtp
} // NS Network