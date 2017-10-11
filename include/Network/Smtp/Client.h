#pragma once

#include <string>
#include <vector>

#include "Network/Smtp/Address.h"

namespace Network {
	
	namespace Smtp {
		
		class SmtpClientImpl;
		
		class SmtpClient {
		public:
			SmtpClient(const std::string& host, 
				const std::string& port, 
				int connectTimeout, 
				int sendTimeout, 
				int recieveTimeout);
			
			void sendMail(const SmtpAddress from, 
				const std::vector<SmtpAddress>& vec_to, 
				const std::string& subject, 
				const std::string& body) const;
		
		private:
			SmtpClientImpl* _impl;
		};

	} // NS Smtp

} // NS Network