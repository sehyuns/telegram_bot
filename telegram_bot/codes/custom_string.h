#pragma once

#include <string>

class CustomString
{
public:
	static STRING trim(__in STRING& s, __in const STRING& drop = " ");
	static STRING rtrim(__in STRING s, __in const STRING& drop = " ");
	static STRING ltrim(__in STRING s, __in const STRING& drop = " ");
	static STRING trim_custom(__in STRING& s, __in const STRING& drop = " ");
	static STRING replace_all(__in const STRING &message, __in const STRING &pattern, __in const STRING &replace);

	static STRING urlencode(__in const STRING &source);
	static STRING urldecode(__in const STRING &source);
	static STRING urlencode2(__in const STRING &source);

	static STRING UTF8ToAnsi(__in STRING _in);
	static STRING AnsiToUTF8(__in STRING _in);
};
