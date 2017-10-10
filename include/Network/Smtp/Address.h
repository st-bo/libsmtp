#pragma once

#include <string>

namespace Network {

	namespace Smtp {

		class SmtpAddress {

		public:
			SmtpAddress(const std::string& name, const std::string& email);

			const std::string toString() const;

			inline const std::string& getEmail() const { return _email; }

		private:
			std::string _name;
			std::string _email;
		};



	} // NS Smtp

} // NS Network