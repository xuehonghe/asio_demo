
#ifndef message_codec_HPP
#define message_codec_HPP

#include <cstdio>
#include <cstdlib>
#include <cstring>

class message_codec
{
public:
	enum { header_length = 4 };
	enum { max_body_length = 512 };

	message_codec()
		: body_length_(0)
	{
	}

public:
	char* begin()
	{
		return data_;
	}

	const char* begin() const
	{
		return data_;
	}

	std::size_t length() const
	{
		return header_length + body_length_;
	}

	char* body()
	{
		return data_ + header_length;
	}

	const char* body() const
	{
		return data_ + header_length;
	}

	std::size_t body_length() const
	{
		return body_length_;
	}

	bool decode_header()
	{
		//char header[header_length + 1] = "";
		//std::strncat(header, data_, header_length);
		//body_length_ = std::atoi(header);

		memcpy(&body_length_, data_, sizeof(body_length_));

		if (body_length_ > max_body_length)
		{
			body_length_ = 0;
			return false;
		}
		return true;
	}

	void decode_body(char& cmd_id, const char** msg, unsigned short& data_len)
	{
		int index = 0;
		const char* data = this->body();
		cmd_id = data[index];
		index += sizeof(cmd_id);

		memcpy(&data_len, data + index, sizeof(data_len));
		index += sizeof(data_len);

		*msg = data + index;
	}


public:
	void encode(char cmd_id, const char* data, unsigned short data_len)
	{
		encode_body(cmd_id, data, data_len);
		encode_header();
	}

protected:

	void encode_header()
	{
		char header[header_length + 1] = "";
		std::memcpy(data_, &body_length_, header_length);
		//std::sprintf(header, "%4d", static_cast<int>(body_length_));
		//std::memcpy(data_, header, header_length);
	}

	void encode_body(char cmd_id, const char* data, unsigned short data_len)
	{
		int body_len = 0;
		char* buf = this->body();

		std::memcpy(buf + body_len, &cmd_id, sizeof(cmd_id));
		body_len += sizeof(cmd_id);

		std::memcpy(buf + body_len, &data_len, sizeof(data_len));
		body_len += sizeof(data_len);

		std::memcpy(buf + body_len, data, data_len);
		body_len += data_len;

		this->set_body_length(body_len);
	}

	void set_body_length(std::size_t new_length)
	{
		body_length_ = new_length;
		if (body_length_ > max_body_length)
		{
			body_length_ = max_body_length;
		}
	}

private:
	char data_[header_length + max_body_length];
	std::size_t body_length_;
};

#endif // message_codec_HPP
