
#include "stdafx.h"
#include "telegram_connector.h"
#include "custom_STRING.h"

#include "json/json.h"


#define		API_URL			"https://api.telegram.org/bot"
#define		GET_ME			"/getme"
#define		GET_UPDATES		"/getUpdates"
#define		SEND_MESSAGE	"/sendMessage?"


namespace Main
{
	
	string		TelegramConnector::_last_message;
	string		TelegramConnector::_bot_identity;
	FILE*		TelegramConnector::_file[TYPE_COUNT];

	////////////////////////////////////////////////////////////////////////////////////////////////
	//! 
	////////////////////////////////////////////////////////////////////////////////////////////////
	TelegramConnector::TelegramConnector() : _curl(nullptr), _last_update(0)
	{
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	//! 
	////////////////////////////////////////////////////////////////////////////////////////////////
	BOOL TelegramConnector::initialize(VOID)
	{
		_curl = curl_easy_init();
		if (nullptr == _curl)
		{
			return false;
		}

		char current_dir[1024] = { 0, };
		GetCurrentDirectoryA(1024, current_dir);

		string file_name = current_dir;
		file_name.append("\\");
		file_name.append("bot_seq.txt");

		char buffer[1024] = { 0, };
		_file[SEQ_FILE] = fopen(file_name.c_str(), "a+");
		fread(buffer, 1, 1024, _file[SEQ_FILE]);

		_last_update = atoi(buffer);

		file_name = current_dir;
		file_name.append("\\");
		file_name.append("bot_id.txt");

		_file[ID_FILE] = fopen(file_name.c_str(), "a+");
		fgets(buffer, 1024, _file[ID_FILE]);

		_bot_identity = buffer;

		// Get Me
		string recv_data;
		string request = API_URL;
		request.append(_bot_identity);
		request.append(GET_ME);

		curl_easy_setopt(_curl, CURLOPT_URL, request.c_str());
		curl_easy_setopt(_curl, CURLOPT_SSL_VERIFYPEER, false);
		curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, http_init_data);
		curl_easy_setopt(_curl, CURLOPT_WRITEDATA, recv_data);
		const CURLcode result = curl_easy_perform(_curl);
		if (CURLE_OK != result)
		{
			cerr << "Error from cURL: " << curl_easy_strerror(result) << endl;
			return false;
		}

		send_message(91271537, "시작합니다.");

		return true;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	//! 
	////////////////////////////////////////////////////////////////////////////////////////////////
	VOID TelegramConnector::close(VOID)
	{
		if (nullptr != _curl)
		{
			curl_easy_cleanup(_curl);
		}

		fclose(_file[SEQ_FILE]);
		fclose(_file[ID_FILE]);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	//! 
	////////////////////////////////////////////////////////////////////////////////////////////////
	VOID TelegramConnector::monitor(VOID)
	{
		parse_string();

		string recv_data;
		string request = API_URL;
		request.append(_bot_identity);
		request.append(GET_UPDATES);

		curl_easy_setopt(_curl, CURLOPT_URL, request.c_str());
		curl_easy_setopt(_curl, CURLOPT_SSL_VERIFYPEER, false);
		curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, http_receive_data);
		curl_easy_setopt(_curl, CURLOPT_WRITEDATA, recv_data);
		const CURLcode result = curl_easy_perform(_curl);
		if (CURLE_OK != result)
		{
			cerr << "Error from cURL: " << curl_easy_strerror(result) << endl;
		}

	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	//! 
	////////////////////////////////////////////////////////////////////////////////////////////////
	VOID TelegramConnector::update_last_seq(INT32 seq_in)
	{
		_last_update = seq_in;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	//! 
	////////////////////////////////////////////////////////////////////////////////////////////////
	BOOL TelegramConnector::validate_last_seq(INT32 seq_in)
	{
		if (_last_update < seq_in)
			return true;
		return false;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	//! 
	////////////////////////////////////////////////////////////////////////////////////////////////
	VOID TelegramConnector::send_message(INT32 chat_id_in, string message_in)
	{
		string request = API_URL;
		request.append(_bot_identity);
		request.append(SEND_MESSAGE);

		message_in = CustomString::AnsiToUTF8(message_in);

		string encoding_STRING = CustomString::replace_all(message_in, "\n", "%20");

		request += "chat_id=" + std::to_string(chat_id_in) + "&";
		request += "text=" + encoding_STRING;

		curl_easy_reset(_curl);
		curl_easy_setopt(_curl, CURLOPT_URL, request.c_str());
		curl_easy_setopt(_curl, CURLOPT_SSL_VERIFYPEER, false);
		curl_easy_setopt(_curl, CURLOPT_FORBID_REUSE, true);
		curl_easy_setopt(_curl, CURLOPT_FRESH_CONNECT, true);

		const CURLcode result = curl_easy_perform(_curl);
		if (CURLE_OK != result)
		{
			cerr << "Error from cURL: " << curl_easy_strerror(result) << endl;
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	//! 
	////////////////////////////////////////////////////////////////////////////////////////////////
	SIZE_T	TelegramConnector::http_init_data(char* contents, SIZE_T size, SIZE_T nmemb, VOID* user_data)
	{
		string contents_str = contents;

		cout << contents_str.c_str() << endl;

		Json::Reader reader;
		Json::Value root;
		if (!reader.parse(contents_str, root))
		{
			cerr << "failed read json result!!" << endl;
		}

		Json::Value result = root["result"];
		cout << "id: " << CustomString::UTF8ToAnsi(result["id"].asString()) << endl;
		cout << "first name: " << CustomString::UTF8ToAnsi(result["first_name"].asString()) << endl;
		cout << "user name: " << CustomString::UTF8ToAnsi(result["username"].asString()) << endl;

		return size * nmemb;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	//! 
	////////////////////////////////////////////////////////////////////////////////////////////////
	SIZE_T	TelegramConnector::http_receive_data(char* contents, SIZE_T size, SIZE_T nmemb, VOID* user_data)
	{

		_last_message = contents;
		string contents_str = contents;

		return size * nmemb;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	//! 
	////////////////////////////////////////////////////////////////////////////////////////////////
	VOID TelegramConnector::parse_string(VOID)
	{
		Json::Reader reader;
		Json::Value root;
		if (!reader.parse(_last_message, root, false))
		{
			cerr << "failed read json result!!" << endl;
			return;
		}

		Json::Value result = root["result"];

		int update_id = 0;
		for (auto ii : result)
		{
			update_id = ii["update_id"].asInt();
			if (!validate_last_seq(update_id))
			{
				update_id = 0;
				continue;
			}

			cout << "update id: " << ii["update_id"] << endl;
			Json::Value message = ii["message"];
			cout << "message id: " << message["message_id"] << endl;

			Json::Value chat = message["chat"];
			cout << "chat id: " << chat["id"] << endl;

			string msg = CustomString::UTF8ToAnsi(message["text"].asString());
			cout << "message: " << msg << endl;

			parse_command(msg);

			update_last_seq(update_id);
		}

		if (0 != update_id)
		{
			fpos_t position = 0;
			fsetpos(_file[SEQ_FILE], &position);
			string seq = to_string(update_id);
			fputs(seq.c_str(), _file[SEQ_FILE]);
			fflush(_file[SEQ_FILE]);
		}
		return VOID();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	//! 
	////////////////////////////////////////////////////////////////////////////////////////////////
	VOID TelegramConnector::parse_command(string & command)
	{
		SIZE_T pos = string::npos;
		command = CustomString::ltrim(command);

		if (command[0] != '/')
			return;

		pos = command.find(" ");
		string cmd = command.substr(1, pos);

		if ("search" == cmd)
		{
			string argument = command.substr(pos + 1);

			cerr << cmd << ", " << argument << endl;
		}
		return VOID();
	}
}