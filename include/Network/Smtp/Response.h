#pragma once

#include <string>

#include "Network\Smtp\Status.h"

namespace Network {

	namespace Smtp {

		typedef struct Response_ {
			std::string bytes;
			Status      status;
		} Response;

		const Response parseResponse(const std::string& payload);

		class ParseResponseException : public std::exception {
		public:
			ParseResponseException(const std::string& payload) {
				_payload = payload;
			}

		private:
			std::string _payload;
		};

	}
}