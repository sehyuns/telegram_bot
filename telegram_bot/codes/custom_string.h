#pragma once

#include <string>

class CustomString
{
public:
	static string trim(__in string& s, __in const string& drop);
	static string rtrim(__in string s, __in const string& drop);
	static string ltrim(__in string s, __in const string& drop);
	static string trim_custom(__in string& s, __in const string& drop);
	static string replace_all(__in const string &message, __in const string &pattern, __in const string &replace);

	static string urlencode(__in const string &source);
	static string urldecode(__in const string &source);
	static string urlencode2(__in const string &source);

	static string UTF8ToAnsi(__in string _in);
	static string AnsiToUTF8(__in string _in);
};
