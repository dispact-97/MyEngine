#pragma once

#include <sys/stat.h>
#include <string>


namespace Util
{
	// ���� üĿ
	bool check_dir_exist(const char* path);
	std::wstring strtoWstr(const std::string& str,std::wstring& wstr);
}
