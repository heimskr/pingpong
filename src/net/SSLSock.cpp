#include <iostream>
#include <stdexcept>

#include "pingpong/core/Debug.h"
#include "pingpong/net/SSLSock.h"
#include "pingpong/net/NetError.h"

namespace PingPong::Net {
	void SSLSock::connect() {
		Sock::connect();
		connectSSL();
	}

	ssize_t SSLSock::send(const void *data, size_t bytes) {
		if (!connected)
			throw std::invalid_argument("Socket not connected");
		size_t written;
		const int status = SSL_write_ex(ssl, data, bytes, &written);
		if (status == 1)
			return static_cast<ssize_t>(written);
		return -SSL_get_error(ssl, status);
	}

	ssize_t SSLSock::recv(void *data, size_t bytes) {
		if (!connected)
			throw std::invalid_argument("Socket not connected");

		fd_set fds_copy = fds;
		int status = select(FD_SETSIZE, &fds_copy, NULL, NULL, NULL);
		if (status < 0) {
			DBG("select status: " << strerror(status));
			throw PingPong::Net::NetError(errno);
		}
			
		if (FD_ISSET(netFD, &fds_copy)) {
			bool read_blocked;
			size_t bytes_read, total_bytes_read = 0;
			int ssl_error;
			do {
				read_blocked = false;
				status = SSL_read_ex(ssl, data, bytes, &bytes_read);

				total_bytes_read += bytes_read;
				if (bytes <= bytes_read)
					bytes = 0;
				else
					bytes -= bytes_read;

				if (status == 0)
					switch (ssl_error = SSL_get_error(ssl, status)) {
						case SSL_ERROR_NONE:
							DBG("SSL_ERROR_NONE");
							return bytes_read;
							break;
						
						case SSL_ERROR_ZERO_RETURN:
							DBG("SSL_ERROR_ZERO_RETURN");
							close();
							break;
						
						case SSL_ERROR_WANT_READ:
							DBG("SSL_ERROR_WANT_READ");
							read_blocked = true;
							break;
						
						case SSL_ERROR_WANT_WRITE:
							DBG("SSL_ERROR_WANT_WRITE");
							break;
						
						case SSL_ERROR_SYSCALL:
							DBG("SSL_ERROR_SYSCALL");
							close();
							break;
		
						default:
							DBG("default");
							close();
							break;
					}
			} while (SSL_pending(ssl) && !read_blocked && 0 < bytes);

			return total_bytes_read;
		} else if (FD_ISSET(controlRead, &fds_copy)) {
			ControlMessage message;
			status = ::read(controlRead, &message, 1);
			if (status < 0) {
				DBG("control_fd status: " << strerror(status));
				throw PingPong::Net::NetError(errno);
			}

			if (message != ControlMessage::Close) {
				DBG("Unknown control message: '" << static_cast<char>(message) << "'");
			}

			SSL_free(ssl);
			::close(netFD);
			SSL_CTX_free(sslContext);
			return 0;
		} else {
			DBG("No file descriptor is ready.");
		}

		return -1;
	}

	void SSLSock::connectSSL() {
		const SSL_METHOD *method = TLS_client_method();
		sslContext = SSL_CTX_new(method);

		if (!sslContext) {
			ERR_print_errors_fp(stderr);
			throw std::runtime_error("SSLSock::connectSSL failed");
		}

		ssl = SSL_new(sslContext);
		if (!ssl)
			throw std::runtime_error("SSLSock::connectSSL: SSL_new failed");

		SSL_set_fd(ssl, netFD);

		const int status = SSL_connect(ssl);
		if (status != 1) {
			ERR_print_errors_fp(stderr);
			throw std::runtime_error("SSLSock::connectSSL: SSL_connect failed ("
				+ std::to_string(SSL_get_error(ssl, status)) + ")");
		}

		DBG("Connected with " << SSL_get_cipher(ssl));

		X509 *cert = SSL_get_peer_certificate(ssl);
		if (cert != nullptr) {
			DBG("Server");
			char *line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
			DBG("Subject: " << line);
			free(line);
			line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
			DBG("Issuer: " << line);
			free(line);
			X509_free(cert);
		} else
			DBG("No client certificates configured.");
	}
}
