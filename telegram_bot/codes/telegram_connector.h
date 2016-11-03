///////////////////////////////////////////////////////////////////////////
//! @file	telegram_connector.h
//! @brief	텔레그램과 통신을 하는 커넥터.
//! @author	sehyun, cho
///////////////////////////////////////////////////////////////////////////

#pragma once

#include "singleton.h"

#include "libcurl\include\curl.h"


namespace Main
{
	///////////////////////////////////////////////////////////////////////////
	//! @class	TelegramConnector
	//! @brief	싱글턴으로서 텔레그램과 통신을 하기 위한 모듈.
	//! 
	///////////////////////////////////////////////////////////////////////////
	class TelegramConnector : public Singleton<TelegramConnector>
	{
	public:
		//! @brief	생성자
		TelegramConnector();

	public:
		//! @brief	초기화 함수
		//! @return	성공시, true
		BOOL		initialize(VOID);
		//! @brief	종료 함수
		VOID		close(VOID);

	public:
		//! @brief	모니터링 루프
		VOID		monitor(VOID);

		//! @brief	메세지를 텔레그램으로 보낸다.
		//! @param	chat_id_in[IN]	보낼 채팅방의 아이디
		//! @param	message_in[IN]	보낼 메세지. 이는 encoding 이 완료된 메세지어야 함.
		VOID		send_message(INT32 chat_id_in, string message_in);

	public:
		//! @brief	GET/SET Functions
		VOID		update_last_seq(INT32 seq_in);
		BOOL		validate_last_seq(INT32 seq_in);

	protected:
		//! @brief	텔레그램으로부터 도착한 메세지를 처리하기 위한 콜백. 초기화 메세지를 처리함.
		//! @param	contents_in[IN]	도착한 메세지 포인터
		//! @param	size_in[IN]		메세지의 데이터 타입 크기
		//! @param	nmemb_in[IN]	메세지의 길이
		//! @param	user_data_in[IN]
		static SIZE_T	http_init_data(char* contents_in, SIZE_T size_in, SIZE_T nmemb_in, VOID* user_data_in);
		static SIZE_T	http_receive_data(char* contents_in, SIZE_T size_in, SIZE_T nmemb_in, VOID* user_data_in);


	private:
		CURL*		_curl;
		INT32		_last_update;
	};

}