#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <regex>
#include "include/rapidjson/document.h"
#include "include/rapidjson/writer.h"
#include "include/rapidjson/stringbuffer.h"
#include "include/rapidjson/filereadstream.h"
#include "downloader.cpp"
#include "vars.cpp"
#include "regexs.cpp"


struct lzl{
	int num = -1;
	std::string pid = "EMPTY!";
	std::string content = "EMPTY!";
};


class lzls{
	lzl lzl_s[31];
	int lengh = 0;

	public:
	int get_nums(int i){return lzl_s[i].num;}
	std::string get_pids(int i){return lzl_s[i].pid;}
	std::string get_contents(int i){return lzl_s[i].content;}

	lzl get(std::string target_pid){ // 想着用二分的，不过问题多多就算了......以后要提高性能这里可以改二分
		for(int i=0;i<= lengh;i++){
			if(lzl_s[i].pid == target_pid)return lzl_s[i];
		}
		return {-1,"EMPTY!",""};
	}

	int set_nums(int i, int value){
		if(lzl_s[i].content != "EMPTY!" && lzl_s[i].pid != "EMPTY!")lengh++;
		lzl_s[i].num = value;
		return lzl_s[i].num;
	}
	std::string set_pids(int i, std::string value){
		if(lzl_s[i].num != -1 && lzl_s[i].content != "EMPTY!")lengh++;
		lzl_s[i].pid = value;
		return lzl_s[i].pid;
	}
	std::string set_contents(int i, std::string value){
		if(lzl_s[i].num != -1 && lzl_s[i].pid != "EMPTY!")lengh++;
		lzl_s[i].content = value;
		return lzl_s[i].content;
	}
};

class lzls_getter{
	std::string path_file;
	std::string pn;
	lzls lzl_list;
	std::ifstream inf;

	std::string get_more(std::string pid){
		std::string s_res = "";// 用于在写入文件前保存提取的文本,并进一步处理
		std::string lzl_pn = "0";// 楼中楼页数
		std::ifstream lzlin;

		///////////////////////////////////////////////////////////////////////////////
		//获取楼中楼页数

		string path = downloader::get("lzl", 0, pid);
		lzlin.open(path+global_vars::name+"_lzl_"+pid+"_1");

		if(!lzlin.good()){
			cerr  << "[" << time(0) << "]  " << "Open lzl input file: " << (path+global_vars::name+"_lzl_"+pid+"1") << " failed" << endl;
			exit(-10);
		}

		while (!lzlin.eof()){
			getline(lzlin, global_vars::linein);
			if (regex_search(global_vars::linein, regs::res, regs::rnum)){
				lzl_pn = regs::res[1];
			}
		}
		cout  << "[" << time(0) << "]  " << "lzl_pn:" << lzl_pn << endl;
		
		lzlin.close();

		if(lzl_pn == "0")return "";

		//////////////////////////////////////////////////////////////////////////////
		// 获取页数完毕，开始提取数据。
		path = downloader::get("lzl", 1, pid, lzl_pn);
		for(int j=1;j <= stoi(lzl_pn);j++){

			#define LZL_INFILE (path + global_vars::name + "_lzl_" + pid + "_" + to_string(j))
			lzlin.open(LZL_INFILE);
			if(!lzlin.good()){
				cerr  << "[" << time(0) << "]  " << "Open lzl input file: " << LZL_INFILE << " failed" << endl;
				exit(-10);
			}


			while(!lzlin.eof()){
				string lzls[11];
				getline(lzlin, global_vars::linein);
				sregex_token_iterator it(global_vars::linein.begin(), global_vars::linein.end(), regs::lzl);
				sregex_token_iterator end;
				int len=0;
				for (;it!=end;++it){
					lzls[len] = it->str();
					len++;
				}
				for(int i=0;i<len;i++){
					if(regex_search(lzls[i], regs::res, regs::lzl_username)){
						s_res += "\n>\n> ###### ";
						s_res += regs::res[1];
						s_res += " ";
					}
					if(regex_search(lzls[i], regs::res, regs::reply)){
						s_res += "回复 ";
						s_res += regs::res[1];
						s_res += " :  \n>";
						if(regex_search(lzls[i], regs::res, regs::con_rp)){
							s_res += regs::res[1];
							s_res = regex_replace(s_res, regs::br,"  \n>");
							if(j != stoi(lzl_pn))s_res += "  \n>";
						}
					}else{
						s_res += ":  \n>";
						if(regex_search(lzls[i], regs::res, regs::con_nrp)){
							s_res += regs::res[1];
							s_res = regex_replace(s_res, regs::br,"  \n>");
							if(j != stoi(lzl_pn))s_res += "  \n>";
						}
					}
				}
			}

			lzlin.close();
		}
		return s_res;
	}

	public:

	lzls get_pre(){
	
		getline(inf, global_vars::linein);

		rapidjson::Document list;
		list.Parse(global_vars::linein.c_str());
		{
			int i =0 ;
			if(!list["data"]["comment_list"].IsObject()){
				std::cerr << "The page " << pn << "has no comment" <<"\n";
				return lzl_list;
			}
			for(auto& it:list["data"]["comment_list"].GetObject()){
				lzl_list.set_pids(i, it.name.GetString());
				lzl_list.set_nums(i, it.value["comment_num"].GetInt());
				if(lzl_list.get_nums(i)<=10||global_vars::is_local){
					std::string temp;
					for(auto& j:it.value["comment_info"].GetArray()){
						{
							rapidjson::Value::ConstMemberIterator itr = list["data"]["user_list"].FindMember((to_string(j["user_id"].GetInt64())).c_str());
							if(itr != list["data"]["user_list"].MemberEnd()){
								
								rapidjson::Value::ConstMemberIterator iter = itr->value.FindMember("user_nickname");
								if(iter != itr->value.MemberEnd()){
									std::string t = (itr->value["user_nickname"].GetString());
									temp += ">\n> ###### " + t;
								}else{
									std::string t = (itr->value["user_name"].GetString());
									temp += ">\n> ###### " + t;
								}
								// 我已经晕了QAQ，指来指去我都分不清了，不过也不知道咋写......
							}else{
								std::cerr << "Have a wrong json! in Parser.cpp:174" << "\n";
							}
						}

						std::string te = j["content"].GetString();

						if(regex_search(te, regs::res, regs::huifu)){
							temp += te;
							std::string t=regs::res[1];
							temp = regex_replace(temp, regs::huifu, " 回复 " + t +": \n> ");
							temp += "\n";
						}else{
							temp += ":  \n> ";
							temp += te;
							temp += "\n";
						}
					}

					lzl_list.set_contents(i, temp);
				}else{
					lzl_list.set_contents(i, get_more(lzl_list.get_pids(i)));
				}
				i++;
			}
		}
		return lzl_list;
	}

	lzls_getter(std::string pgnum){
		pn = pgnum;
		path_file = global_vars::path + global_vars::name + "/lzl/";
		if(!global_vars::is_local)downloader::get("lzl_list", false, pn);
		inf.open((path_file + global_vars::name + pn + "_list.json"));
		if(!inf.good()){
			std::cerr << "[" << time(0) << "] open file " << (path_file + global_vars::name + pn + "_list.json") << "faild in./Parser.cpp:207\n";
			exit(-10);
		}
		lzl_list = get_pre();
	}

	lzls get(){
		return lzl_list;
	}

};

class parser{
	std::string path_file;

	string get_page_number(){
		downloader::get("content", false);

		global_vars::in.open(path_file + global_vars::name + "_all1.brysource");
		if(!global_vars::in.good()){
			cerr  << "[" << time(0) << "]  " << "Open input file: " << (path_file + global_vars::name + "_all1.brysource") << " failed in Parser.cpp:235" << endl;
			exit(-10);
		}

		string s_res = "0";

		while (!global_vars::in.eof()){
			getline(global_vars::in, global_vars::linein);
			if (regex_search(global_vars::linein, regs::res, regs::rnum)){
				s_res = regs::res[0];
				s_res = regex_replace(s_res, regs::rnum_e, "");
			}
		}

		if(s_res == "0")return "can't get pn";

		global_vars::in.close();

		return s_res;
	}

	public:

	parser(){//构造函数（就是填下路径）
		path_file = global_vars::path + global_vars::name +"/source/";
	}

	std::string all_to_file(){ // 下载并提取帖子内容，写入在settings.json里指定的路径下的 name
							   //（同样在settings.json里指定）文件夹，返回值是帖子页数
		std::string pn;
		global_vars::is_local? pn = global_vars::l_pn : pn = get_page_number();

		std::string s_res = "";// 用于在写入文件前保存提取的文本,并进一步处理


		cout  << "[" << time(0) << "]  " << "pn:" << pn << endl;

		if(!global_vars::is_local)downloader::get("content", true, pn);

		for (int i = 1; i <= stoi(pn); i++){
			#define INFILE path_file + global_vars::name + "_all" + to_string(i) + ".brysource"
			#define OUTFILE path_file + "../" + global_vars::name + to_string(i) + ".md"

			global_vars::in.open(INFILE);
			global_vars::out.open(OUTFILE);
			lzls_getter lzl_getter(to_string(i));

			// 异常处理（写入日志）
			if(!global_vars::in.good()){
				cerr  << "[" << time(0) << "]  " << "Open input file: " << INFILE << " failed in Parser.cpp:270" << endl;
				exit(-10);
			}
			if(!global_vars::out.good()){
				cerr  << "[" << time(0) << "]  " << "Open output file: "<< OUTFILE << " failed in Parser.cpp:271" << endl;
				exit(-11);
			}


			while (!global_vars::in.eof()){
				getline(global_vars::in, global_vars::linein);
				// 提取用户名
				if(regex_search(global_vars::linein, regs::res, regs::username)){
					s_res = "\n##### ";
					s_res += regs::res[1];
					s_res += "  \n";
				}
				// 提取正文
				// 正文格式有两种，一种是带装饰框的，一种不是，所以用if-else if来匹配
				if (regex_search(global_vars::linein, regs::res, regs::pop_content)){
					s_res += "&emsp;";
					s_res += regs::res[1];
					s_res = regex_replace(s_res, regs::br,"  \n");
				}else{
					if (regex_search(global_vars::linein, regs::res, regs::content)){
						s_res += regs::res[1];
						s_res = regex_replace(s_res, regs::br,"  \n");
						s_res = regex_replace(s_res, regs::linehand, "&emsp;");
					}
				}
				// 提取楼层数
				if(regex_search(global_vars::linein, regs::res, regs::flnum)){
					s_res += "  \n&emsp;";
					s_res += regs::res[1];
					s_res += "  \n";
				}
				// 提取发帖时间
				if(regex_search(global_vars::linein, regs::res, regs::fltime)){
					s_res += "&emsp;";
					s_res += regs::res[0];
				}
				// 提取楼中楼
				if(regex_search(global_vars::linein, regs::res, regs::pid)){
					string pid = regs::res[0];
					pid = regex_replace(pid, regs::pid_h, "");
					pid = regex_replace(pid, regs::pid_e, "");
					s_res += lzl_getter.get().get(pid).content;
					s_res += "  \n***";
					global_vars::out << s_res ;// 写入文件
				}
			}

			global_vars::in.close();
			global_vars::out.close();
		}

		return pn;
	}

	std::string content(){ // 只提取帖子正文（不提取楼中楼）以std::string形式返回
		std::string pn;
		global_vars::is_local? pn = global_vars::l_pn : pn = get_page_number();

		std::string s_res = "";// 提取的内容

		cerr  << "[" << time(0) << "]  " << "pn:" << pn << endl;

		if(!global_vars::is_local)downloader::get("content", true, pn);

		for (int i = 1; i <= stoi(pn); i++){
			#define INFILE path_file + global_vars::name + "_all" + to_string(i) + ".brysource"
			global_vars::in.open(INFILE);
			lzls_getter lzl_getter(to_string(i));

			// 异常处理（写入日志）
			if(!global_vars::in.good()){
				cerr  << "[" << time(0) << "]  " << "Open input file: " << INFILE << " failed in Parser.cpp:340" << endl;
				exit(-10);
			}

			while (!global_vars::in.eof()){
				getline(global_vars::in, global_vars::linein);
				// 提取用户名
				if(regex_search(global_vars::linein, regs::res, regs::username)){
					s_res = "\n##### ";
					s_res += regs::res[1];
					s_res += "  \n";
				}
				// 提取正文
				// 正文格式有两种，一种是带装饰框的，一种不是，所以用if-else if来匹配
				if (regex_search(global_vars::linein, regs::res, regs::pop_content)){
					s_res += "&emsp;";
					s_res += regs::res[1];
					s_res = regex_replace(s_res, regs::br,"  \n");
				}else{
					if (regex_search(global_vars::linein, regs::res, regs::content)){
						s_res += regs::res[1];
						s_res = regex_replace(s_res, regs::br,"  \n");
						s_res = regex_replace(s_res, regs::linehand, "&emsp;");
					}
				}
				// 提取楼层数
				if(regex_search(global_vars::linein, regs::res, regs::flnum)){
					s_res += "  \n&emsp;";
					s_res += regs::res[1];
					s_res += "  \n";
				}
				// 提取发帖时间
				if(regex_search(global_vars::linein, regs::res, regs::fltime)){
					s_res += "&emsp;";
					s_res += regs::res[0];
				}
				s_res += "  \n***";
			}

			global_vars::in.close();
		}

		return s_res;
	}

	std::string all(std::string pn){return "该功能还未实现";}//获取某一页的内容
	// 本来是像把all_to_file做下拆分，不过还是懒了（导致all_to_file写的又臭又长......）

};
