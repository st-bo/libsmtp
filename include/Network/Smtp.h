#pragma once

#include <string>

namespace Network {

	namespace Smtp {

		enum Status {
			Ok,
			Fail
		};

		class SmtpAddress {

		public:
			SmtpAddress(const std::string& name, const std::string& email);

			const std::string toString() const;

			inline const std::string& getEmail() const { return _email; }

		private:
			std::string _name;
			std::string _email;
		};

		typedef struct Response_ {
			std::string bytes;
			Status      status;
		} Response;

	} // NS Smtp

} // NS Network