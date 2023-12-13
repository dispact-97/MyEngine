#include "Utility.h"

bool Util::check_dir_exist(const char* path)
{
	// struct stat st;
	// return stat(path, &st) == 0 && ((st.st_mode) & S_IFMT) == S_IFDIR;

	bool exist = false;
	struct stat st;
	if (stat(path, &st) == 0)
	{
		if (((st.st_mode) & S_IFMT) == S_IFDIR)
		{
			exist = true;
		}
	}
	return exist;
}

std::wstring Util::strtoWstr(const std::string& str,std::wstring& wstr)
{
	return wstr = (std::wstring(str.begin(),str.end())).c_str();
}

