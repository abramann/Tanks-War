#include "GameData.h"

namespace nsTextureData
{
	inline Vertex getBodyCenter(const wchar_t* texFileName)
	{
		static std::map<std::wstring, Vertex> bodiesCenters;
		if (bodiesCenters.empty())
		{
			bodiesCenters[L"Hull_01"] = Vertex(127, 175);
			bodiesCenters[L"Gun_01"] = Vertex(46, 154);
		}
		return bodiesCenters[texFileName];
	}
}