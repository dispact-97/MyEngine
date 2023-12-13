#pragma once

#include <sys/stat.h>
#include <string>


namespace Util
{
	// 파일 체커
	bool check_dir_exist(const char* path);
	std::wstring strtoWstr(const std::string& str,std::wstring& wstr);
}
