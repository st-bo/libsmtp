#pragma once

#include <string>

namespace Network {
	
	namespace Smtp {

		namespace Command {
			
			class SmtpCommand {
			public:
				virtual const std::string toString() { return ""; };
			};

			class HeloCommand : public SmtpCommand {
			private:
				std::string _fqdn;
			public:
				HeloCommand(const std::string& fqdn);

				const std::string toString() const;

			};

			class MailFromCommand : public SmtpCommand {
			private:
				std::string _email;
			public:
				MailFromCommand(const std::string& email);

				const std::string toString() const;
			};

			class RecipientToCommand : public SmtpCommand {
			private:
				std::string _email;
			public:
				RecipientToCommand(const std::string& email);

				const std::string toString() const;
			};

			class DataCommand : public SmtpCommand {
			private:
				std::string _payload;
			public:
				DataCommand(const std::string& payload);

				const std::string toString() const;
			};

			class DataFinishedCommand : public SmtpCommand {
			public:
				const std::string toString() const;
			};

			class QuitCommand : public SmtpCommand {
			public:
				const std::string toString() const;
			};

		} // NS Command

	} // NS Smtp

} // NS Network