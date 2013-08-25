#include "JgeStdafx.h"
#include "JgeIO.h"
#include "JgeMemory.h"

bool jge::File::read(const wchar_t* lpFileURL, char* lpFileData, uint* numBytes, bool binary)
{
	std::ifstream reader;
	reader.open(lpFileURL, std::ios_base::in | (binary ? std::ios_base::binary : 0));
	bool r = false;
	if(reader.good())
	{
		reader.seekg(0, std::ios_base::end);
		uint bytes = (uint)reader.tellg();
		reader.seekg(0, std::ios_base::beg);
		if(lpFileData != null)
		{
			*numBytes = bytes;
		}
		if(lpFileData != null)
		{
			jgeZeroMem(lpFileData, bytes);
			reader.read(lpFileData, bytes);
		}
		r = true;
	}
	reader.close();
	return r;
}