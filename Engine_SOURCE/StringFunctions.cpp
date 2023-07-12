#include "StringFunctions.h"
#include "SimpleMath.h"

namespace dru
{
	void SetRandIndex(std::wstring& string, UINT range)
	{
		std::wstring idx =  std::to_wstring(math::GetRandomNumber(1, range));
		string += idx;
	}
	

}