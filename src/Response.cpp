

#include "Network\Smtp\Response.h"


namespace Network {

	namespace Smtp {
		
		const Response parseResponse(const std::string& payload) {
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

	}
}