#include "StringFunctions.h"
#include "SimpleMath.h"
#include "Engine.h"




void GetLastErrorPrint()
{
	DWORD error = GetLastError();
	char* message = nullptr;

	FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
		nullptr,
		error,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&message,
		0,
		nullptr);

	if (nullptr != message)
	{
		std::string Text = "Code : ";
		Text += std::to_string(error);
		Text += " Message : ";
		Text += message;

		MessageBoxA(nullptr, (Text).c_str(), "Error", MB_OK);
		// 운영체제가 준건 해제헤야 한다.
		// 서버면 더더더더더더욱
		// 실행중 발생하는 릭이 가장 위험하다.
		// 힙이 비대해지기 시작합니다.
		// 램을 초과하면서 터진다.
		LocalFree(message);
	}
}


void ToUpper(std::string& text)
{
	for (size_t i = 0; i < text.size(); i++)
	{
		text[i] = static_cast<char>(std::toupper(text[i]));
	}
}

std::string ToUpperReturn(const std::string& text)
{
	std::string NewText = text;
	for (size_t i = 0; i < text.size(); i++)
	{
		NewText[i] = static_cast<char>(std::toupper(NewText[i]));
	}
	return NewText;
}

void AnsiToUniCode(const std::string& text, std::wstring& out)
{
	int Size = MultiByteToWideChar(CP_ACP, 0, text.c_str(), static_cast<int>(text.size()), nullptr, 0);

	if (Size == 0)
	{
		MsgBoxAssert("스트링 변환에 실패했습니다.");
		return;
	}

	out.resize(Size);

	Size = MultiByteToWideChar(CP_ACP, 0, text.c_str(), static_cast<int>(text.size()), &out[0], Size);

	if (Size == 0)
	{
		MsgBoxAssert("스트링 변환에 실패했습니다.");
		return;
	}
}

std::wstring AnsiToUniCodeReturn(const std::string& text)
{
	std::wstring UniCode = L"";
	AnsiToUniCode(text, UniCode);
	return UniCode;
}

void UniCodeToUTF8(const std::wstring& text, std::string& out)
{
	int Size = WideCharToMultiByte(CP_UTF8, 0, text.c_str(), static_cast<int>(text.size()), nullptr, 0, nullptr, nullptr);

	if (Size == 0)
	{
		MsgBoxAssert("스트링 변환에 실패했습니다.");
		return;
	}

	out.resize(Size);

	Size = WideCharToMultiByte(CP_UTF8, 0, text.c_str(), static_cast<int>(text.size()), &out[0], static_cast<int>(out.size()), nullptr, nullptr);

	if (Size == 0)
	{
		MsgBoxAssert("스트링 변환에 실패했습니다.");
		return;
	}
}

void AnsiToUTF8(const std::string& text, std::string& out)
{
	std::wstring WString;

	AnsiToUniCode(text, WString);
	UniCodeToUTF8(WString, out);
}

std::string AnsiToUTF8Return(const std::string& text)
{
	std::wstring WString;
	std::string Result;

	AnsiToUniCode(text, WString);
	UniCodeToUTF8(WString, Result);

	return Result;
}

std::wstring StringToWStringReturn(const std::string& text)
{
	std::wstring Return;

	int Size = MultiByteToWideChar(CP_ACP, 0, text.c_str(), static_cast<int>(text.size()), nullptr, 0);

	if (Size == 0)
	{
		MsgBoxAssert("스트링 변환에 실패했습니다.");
		return L"";
	}

	Return.resize(Size);

	Size = MultiByteToWideChar(CP_ACP, 0, text.c_str(), static_cast<int>(text.size()), &Return[0], Size);

	if (Size == 0)
	{
		MsgBoxAssert("스트링 변환에 실패했습니다.");
		return L"";
	}

	return Return;
}


std::string WStringToStringReturn(const std::wstring& wstr)
{
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);

	std::string Return(size_needed, 0);

	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &Return[0], size_needed, NULL, NULL);

	return Return;
}


bool UniCodeToAnsi(const std::wstring& uniCode, std::string& ansiString)
{
	//_In_ UINT CodePage,
	//	_In_ DWORD dwFlags,
	//	_In_NLS_string_(cchWideChar) LPCWCH lpWideCharStr,
	//	_In_ int cchWideChar,
	//	out_writes_bytes_to_opt_(cbMultiByte, return) LPSTR lpMultiByteStr,
	//	_In_ int cbMultiByte,
	//	_In_opt_ LPCCH lpDefaultChar,
	//	out_opt_ LPBOOL lpUsedDefaultChar

	int32_t Len = WideCharToMultiByte(
		CP_ACP,
		0,
		uniCode.c_str(),
		static_cast<int32_t>(uniCode.size()),
		nullptr,
		0,
		nullptr,
		nullptr
	);


	if (0 >= Len)
	{
		GetLastErrorPrint();
		return false;
	}

	ansiString.resize(Len);

	Len = WideCharToMultiByte(
		CP_ACP,
		0,
		uniCode.c_str(),
		static_cast<int32_t>(uniCode.size()),
		&ansiString[0],
		static_cast<int32_t>(ansiString.size()),
		nullptr,
		nullptr
	);

	return true;
}


bool UTF8ToUniCode(const std::string& UTF8, std::wstring& uniCode)
{
	uniCode.clear();

	int32_t Len = MultiByteToWideChar(
		CP_UTF8,
		0,
		UTF8.c_str(),
		static_cast<int32_t>(UTF8.size()),
		nullptr, // 이부분을 nullptr로 넣어주면 변환 함수로 동작하는게 아니고. 변환길이를 체크하는 함수로 동작한다.
		0
	);

	if (0 >= Len)
	{
		GetLastErrorPrint();
		return false;
	}

	uniCode.resize(Len);

	Len = MultiByteToWideChar(
		CP_UTF8,
		0,
		UTF8.c_str(),
		static_cast<int32_t>(UTF8.size()),
		&uniCode[0],
		Len
	);

	if (0 >= Len)
	{
		GetLastErrorPrint();
		return false;
	}

	return true;
}

std::string UniCodeToAnsiReturn(const std::wstring& uniCode)
{
	std::string Return;
	UniCodeToAnsi(uniCode, Return);
	return Return;
}

std::wstring UTF8ToUniCodeReturn(const std::string& UTF8)
{
	std::wstring Return;
	UTF8ToUniCode(UTF8, Return);
	return Return;
}

std::string UTF8ToAnsiReturn(const std::string& UTF8)
{
	std::wstring UniCode;
	if (false == UTF8ToUniCode(UTF8, UniCode))
	{
		MsgBoxAssert("UTF8 => Ansi ConvertError");
	}
	std::string Return;
	if (false == UniCodeToAnsi(UniCode, Return))
	{
		MsgBoxAssert("UniCode => Ansi ConvertError");
	}
	return Return;
}

std::string Replace(const std::string& text, const std::string_view& oldText, const std::string_view& changeText)
{
	size_t FindIndex = text.find(oldText);

	if (FindIndex == std::string::npos)
	{
		return text;
	}

	std::string Text = text;
	Text = Text.replace(FindIndex, oldText.size(), changeText);
	return Text;

	// ".FBX"
	// ".UserFBX"
}

std::wstring ConvertToW_String(const char* str)
{
	std::string sName = std::string(str);
	std::wstring wName(sName.begin(), sName.end());
	return wName;
}

std::string ConvertToString(const wchar_t* str)
{
	std::wstring sName = std::wstring(str);
	std::string wName(sName.begin(), sName.end());
	return wName;
}