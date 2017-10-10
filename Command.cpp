
#include <string>
#include <sstream>

#include "Defs.h"
#include "Network/Smtp/Command.h"

namespace Network {

	namespace Smtp {

		namespace Command {
			
			HeloCommand::HeloCommand(const std::string& fqdn) {
				_fqdn = fqdn;
			}

			const std::string HeloCommand::toString() const {
				return "HELO " + _fqdn;
			}
			
			MailFromCommand::MailFromCommand(const std::string& email) {
				_email = email;
			}

			const std::string MailFromCommand::toString() const {
				return "MAIL FROM:" + _email + NEWLINE;
			}

			RecipientToCommand::RecipientToCommand(const std::string& email) {
				_email = email;
			}

			const std::string RecipientToCommand::toString() const {
				return "RCPT TO:" + _email + NEWLINE;
			}

			DataCommand::DataCommand(const std::string& payload) {
				_payload = payload;
			}

			const std::string DataCommand::toString() const {
				std::ostringstream  oss;
				oss << "DATA" << NEWLINE << _payload;
				return oss.str();
			}

			const std::string DataFinishedCommand::toString() const {
				std::ostringstream  oss;
				oss << NEWLINE << "." << NEWLINE;
				return oss.str();
			}

			const std::string QuitCommand::toString() const {
				return "QUIT";
			}

		} // NS Command

	} // NS Smtp

} // NS Network
