#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace util {

	class Message {
	public:
		enum { header_length = 4 };

		// Might not need max body length
		//enum { max_body_length = 512 };

		Message(const char* str) :
			body_length_(strlen(str)),
			data_(new char[body_length_+header_length]) {
			encode_header(body_length_, data_);
			std::memcpy(data_ + header_length, str, body_length_);
		}

		Message(const std::string& str) : Message(str.c_str()) {
		}

		~Message() {
			//delete data_;
		}

		const char* data() const {
			return data_;
		}

		std::size_t length() const {
			return header_length + body_length_;
		}

		const char* body() const {
			return data_ + header_length;
		}

		char* body() {
			return data_ + header_length;
		}

		std::size_t body_length() const {
			return body_length_;
		}

		static size_t decode_header(const char* data) {
			char header[header_length + 1] = "";
			strncat_s(header, data, header_length);
			return std::atoi(header);
		}

		static void encode_header(size_t body_length, char* data)
		{
			char header[header_length + 1] = "";
			sprintf_s(header, "%4d", static_cast<int>(body_length));
			std::memcpy(data, header, header_length);
		}

	private:
		std::size_t body_length_;
		char* data_;
	};

}