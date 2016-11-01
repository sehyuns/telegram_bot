// telegram_bot.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "telegram_connector.h"

//////////////////////////////////////////////////////////////////////////
#if _MSC_VER >= 1900
#	pragma comment(lib, "legacy_stdio_definitions.lib")
FILE _iob[] = { *stdin, *stdout, *stderr };
extern "C" FILE * __cdecl __iob_func(void) {
	return _iob;
}
#endif
//////////////////////////////////////////////////////////////////////////


int main()
{
	curl_global_init(CURL_GLOBAL_ALL);

	TelegramConnector::instance()->initialize();
	
	while (true)
	{
		TelegramConnector::instance()->monitor();
		Sleep(1000);
	}

	TelegramConnector::instance()->close();
	curl_global_cleanup();

	TelegramConnector::desctroy();

    return 0;
}

