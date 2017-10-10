
#include <sstream>

#include "Network/Smtp/Address.h"

namespace Network {

	namespace Smtp {

		SmtpAddress::SmtpAddress(const std::string& name, const std::string& email) {
			_name = name;
			_email = email;
		}

		const std::string SmtpAddress::toString() const {
			std::ostringstream addr;

			if (!_name.empty()) {
				addr << "\"" << _name << "\" ";
			}

			addr << "<" << _email << ">";

			return addr.str();
		}

	} // NS Smtp

} // NS Network