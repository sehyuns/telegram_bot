#include "stdafx.h"
#include "custom_string.h"
#include "libcurl/include/curl.h"

const char HEX2DEC[256] =
{
	/*       0  1  2  3   4  5  6  7   8  9  A  B   C  D  E  F */
	/* 0 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
	/* 1 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
	/* 2 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
	/* 3 */  0, 1, 2, 3,  4, 5, 6, 7,  8, 9,-1,-1, -1,-1,-1,-1,

	/* 4 */ -1,10,11,12, 13,14,15,-1, -1,-1,-1,-1, -1,-1,-1,-1,
	/* 5 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
	/* 6 */ -1,10,11,12, 13,14,15,-1, -1,-1,-1,-1, -1,-1,-1,-1,
	/* 7 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,

	/* 8 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
	/* 9 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
	/* A */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
	/* B */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,

	/* C */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
	/* D */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
	/* E */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
	/* F */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1
};

const char SAFE[256] =
{
	/*      0 1 2 3  4 5 6 7  8 9 A B  C D E F */
	/* 0 */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
	/* 1 */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
	/* 2 */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
	/* 3 */ 1,1,1,1, 1,1,1,1, 1,1,0,0, 0,0,0,0,

	/* 4 */ 0,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1,
	/* 5 */ 1,1,1,1, 1,1,1,1, 1,1,1,0, 0,0,0,0,
	/* 6 */ 0,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1,
	/* 7 */ 1,1,1,1, 1,1,1,1, 1,1,1,0, 0,0,0,0,

	/* 8 */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
	/* 9 */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
	/* A */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
	/* B */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,

	/* C */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
	/* D */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
	/* E */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
	/* F */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0
};

STRING CustomString::trim(__in STRING& s, __in const STRING& drop /*= " " */)
{
	STRING r = s.erase(s.find_last_not_of(drop) + 1);
	return r.erase(0, r.find_first_not_of(drop));
}

STRING CustomString::rtrim(__in STRING s, __in const STRING& drop /*= " " */)
{
	STRING r = s.erase(s.find_last_not_of(drop) + 1);
	return r;
}

STRING CustomString::ltrim(__in STRING s, __in const STRING& drop /*= " " */)
{
	STRING r = s.erase(0, s.find_first_not_of(drop));
	return r;
}

STRING CustomString::trim_custom(__in STRING& s, __in const STRING& drop /*= " " */)
{
	STRING r = s.erase(0, s.find_first_not_of(L" "));
	r = r.erase(0, r.find_first_not_of(L"\n"));
	r = r.erase(r.find_last_not_of(drop) + 1);
	r = r.erase(r.find_last_not_of(L"\n") + 1);
	r = CustomString::replace_all(r, L"\n \n", L"\n");
	r = CustomString::replace_all(r, L"\n\n", L"\n");
	return r.erase(0, r.find_first_not_of(drop));
}

STRING CustomString::replace_all(__in const STRING &message, __in const STRING &pattern, __in const STRING &replace)
{
	STRING result = message;
	STRING::size_type pos = 0;
	STRING::size_type offset = 0;

	while ((pos = result.find(pattern, offset)) != STRING::npos)
	{
		result.replace(result.begin() + pos, result.begin() + pos + pattern.size(), replace);
		offset = pos + replace.size();
	}

	return result;
}

STRING CustomString::urlencode(__in const STRING &source)
{
	STRING result_string;

	char hex[4];
	for (const char &c : source)
	{
		if ((c > 47 && c < 57) ||
			(c > 64 && c < 92) ||
			(c > 96 && c < 123) ||
			c == '-' || c == '.' || c == '_')
			result_string += c;
		else
		{
			sprintf_s(hex, L"%%%02X", (unsigned char)c);
			result_string.append(hex);
		}
	}

	return result_string;
}

STRING CustomString::urldecode(__in const STRING &source)
{
	// Note from RFC1630: "Sequences which start with a percent
	// sign but are not followed by two hexadecimal characters
	// (0-9, A-F) are reserved for future extension"

	const unsigned char * pSrc = (const unsigned char *)source.c_str();
	const int SRC_LEN = source.length();
	const unsigned char * const SRC_END = pSrc + SRC_LEN;
	// last decodable '%' 
	const unsigned char * const SRC_LAST_DEC = SRC_END - 2;

	char * const pStart = new char[SRC_LEN];
	char * pEnd = pStart;

	while (pSrc < SRC_LAST_DEC)
	{
		if (*pSrc == '%')
		{
			char dec1, dec2;
			if (-1 != (dec1 = HEX2DEC[*(pSrc + 1)])
				&& -1 != (dec2 = HEX2DEC[*(pSrc + 2)]))
			{
				*pEnd++ = (dec1 << 4) + dec2;
				pSrc += 3;
				continue;
			}
		}

		*pEnd++ = *pSrc++;
	}

	// the last 2- chars
	while (pSrc < SRC_END)
		*pEnd++ = *pSrc++;

	STRING sResult(pStart, pEnd);
	delete[] pStart;
	return sResult;
}

// STRING UriEncode( const STRING & sSrc )
// {
// 	const char DEC2HEX[16 + 1] = "0123456789ABCDEF";
// 	const unsigned char * pSrc = (const unsigned char *)sSrc.c_str();
// 	const int SRC_LEN = sSrc.length();
// 	unsigned char * const pStart = new unsigned char[SRC_LEN * 3];
// 	unsigned char * pEnd = pStart;
// 	const unsigned char * const SRC_END = pSrc + SRC_LEN;
// 
// 	for ( ; pSrc < SRC_END; ++pSrc )
// 	{
// 		if ( SAFE[*pSrc] )
// 			*pEnd++ = *pSrc;
// 		else
// 		{
// 			// escape this char
// 			*pEnd++ = '%';
// 			*pEnd++ = DEC2HEX[*pSrc >> 4];
// 			*pEnd++ = DEC2HEX[*pSrc & 0x0F];
// 		}
// 	}
// 
// 	STRING sResult( (char *)pStart, (char *)pEnd );
// 	delete[] pStart;
// 	return sResult;
// }

STRING CustomString::urlencode2(__in const STRING &source)
{
	STRING result_string;

	char hex[6];
	for (const char &c : source)
	{
		if ((c > 47 && c < 57) ||
			(c > 64 && c < 92) ||
			(c > 96 && c < 123) ||
			c == '-' || c == '.' || c == '_')
			result_string += c;
		else
		{
			sprintf_s(hex, L"%%25%02X", c);
			result_string.append(hex);
		}
	}

	return result_string;
}


STRING CustomString::UTF8ToAnsi(__in STRING _in)
{
	STRING result;
	int nLength = MultiByteToWideChar(CP_UTF8, 0, _in.c_str(), _in.length() + 1, NULL, NULL);
	BSTR bstrWide = SysAllocStringLen(NULL, nLength);
	MultiByteToWideChar(CP_UTF8, 0, _in.c_str(), _in.length() + 1, bstrWide, nLength);
	nLength = WideCharToMultiByte(CP_ACP, 0, bstrWide, -1, NULL, 0, NULL, NULL);

	char *pszAnsi = new char[nLength];
	WideCharToMultiByte(CP_ACP, 0, bstrWide, -1, pszAnsi, nLength, NULL, NULL);
	SysFreeString(bstrWide);
	result = pszAnsi;
	delete[] pszAnsi;

	return result;
}

STRING CustomString::AnsiToUTF8(__in STRING _in)
{
	STRING result;
	int     nLength, nLength2;
	BSTR    bstrCode;
	char*   pszUTFCode = NULL;

	nLength = MultiByteToWideChar(CP_ACP, 0, _in.c_str(), lstrlenA(_in.c_str()) + 1, NULL, NULL);
	bstrCode = SysAllocStringLen(NULL, nLength);
	MultiByteToWideChar(CP_ACP, 0, _in.c_str(), lstrlenA(_in.c_str()), bstrCode, nLength);

	nLength2 = WideCharToMultiByte(CP_UTF8, 0, bstrCode, -1, pszUTFCode, 0, NULL, NULL);

	pszUTFCode = new char[nLength2 + 1];
	memset(pszUTFCode, 0, nLength2 + 1);
	WideCharToMultiByte(CP_UTF8, 0, bstrCode, -1, pszUTFCode, nLength2 - 4, NULL, NULL);

	result = pszUTFCode;
	delete[] pszUTFCode;

	return result;
}
