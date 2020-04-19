#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "include/rapidjson/document.h"
#include "include/rapidjson/writer.h"
#include "include/rapidjson/stringbuffer.h"
#include "include/rapidjson/filereadstream.h"
#include "Parser.cpp"
#include "downloader.cpp"
#include "vars.cpp"


using namespace rapidjson;

void getsetting (){
	#ifndef WIN
	FILE* fp = fopen("./settings.json", "r"); // 非 Windows 平台使用 "r"
	#else
	FILE* fp = fopen("./settings.json", "rb"); // 非 Windows 平台使用 "r"
	#endif

	char readBuffer[65536];
	FileReadStream is(fp, readBuffer, sizeof(readBuffer));

	Document d;
	d.ParseStream(is);
	
	global_vars::path = d["path"].GetString();
	global_vars::path_python = d["path_to_python"].GetString();
	global_vars::is_local = d["is_local"].GetBool();
	global_vars::l_pn = d["local_files_page_number"].GetString();
	global_vars::tid = d["tid"].GetString();
	global_vars::name = d["name"].GetString();
	fclose(fp);
}
 
int main() {
	freopen("./log","a",stderr);

	std::ofstream out;
	getsetting();
	parser parser;

	parser.all_to_file();

	std::cout<< "[" << time(0) << "] DONE!\n";

    return 0;
}