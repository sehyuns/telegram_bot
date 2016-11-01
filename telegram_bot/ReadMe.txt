========================================================================
    CONSOLE APPLICATION : telegram_bot Project Overview
========================================================================

AppWizard has created this telegram_bot application for you.

This file contains a summary of what you will find in each of the files that
make up your telegram_bot application.


telegram_bot.vcxproj
    This is the main project file for VC++ projects generated using an Application Wizard.
    It contains information about the version of Visual C++ that generated the file, and
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

telegram_bot.vcxproj.filters
    This is the filters file for VC++ projects generated using an Application Wizard. 
    It contains information about the association between the files in your project 
    and the filters. This association is used in the IDE to show grouping of files with
    similar extensions under a specific node (for e.g. ".cpp" files are associated with the
    "Source Files" filter).

telegram_bot.cpp
    This is the main application source file.

/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named telegram_bot.pch and a precompiled types file named StdAfx.obj.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" comments to indicate parts of the source code you
should add to or customize.

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
Code Convention

1. 기본적으로 소문자 + 언더바로 표기한다.
2. 클래스명은 예외적으로 낙타를 사용한다.(C로 시작하지 말것.)
3. 함수명 및 변수의 맴버 여부는 앞의 언더바로 구분한다.
4. 함수 인자의 경우는 _in, _out 을 끝에 붙여서 인자의 성격을 알려준다.
5. TCHAR 를 기본으로 사용한다.(이와 관련된 API 들도 모두 포함.) string의 경우 typedef basic_string<TCHAR> STRING 을 사용한다.
6. 데이터 타입은 윈도우 표준인 대문자로 사용한다.(INT8, INT16, INT32, INT64, 등등)
7. doxygen 형식의 주석으로 코드를 문서화 한다.

/////////////////////////////////////////////////////////////////////////////