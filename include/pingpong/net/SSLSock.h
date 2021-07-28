#pragma once

#include <cerrno>
#include <netdb.h>
#include <resolv.h>
#include <string.h>
#include <unistd.h>
#include <openssl/err.h>
#include <openssl/ssl.h>

#include "pingpong/net/Sock.h"

namespace PingPong::Net {
	class SSLSock: public Sock {
		public:
			using Sock::Sock;
			void connect() override;
			ssize_t send(const void *, size_t) override;
			ssize_t recv(void *, size_t) override;

		protected:
			SSL_CTX *sslContext = nullptr;
			SSL *ssl = nullptr;

			void connectSSL();
	};
}
