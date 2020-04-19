#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include "vars.cpp"

using namespace std;

namespace downloader{

    std::string get(std::string mode, bool unfirst, std::string pid_or_num = "0", std::string num = "0"){
        // 参数含义：当mode为content，unfirst为false时，依次为mode unfirst 
        //          当mode为content，unfirst为true时，依次为mode unfirst num
        //          当mode为lzl，unfirst为false时，依次为mode unfirst pid
        //          当mode为lzl，unfirst为true时，依次为mode unfirst pid num
        // 返回值是下载文件的路径
        #ifdef WIN
        #define p (global_vars::path_python + " .\\downloader.py " + \
        mode + " " + to_string(unfirst) + " " + global_vars::tid + " " + pid_or_num).c_str()

        #define p2 (global_vars::path_python + " .\\downloader.py " + \
        mode + " " + to_string(unfirst) + " " + global_vars::tid + " " + pid_or_num + " " + num).c_str()
        #endif
        #ifdef LIN
        #define p ("python3 ./downloader.py " + \
        mode + " " + to_string(unfirst) + " " + global_vars::tid + " " + pid_or_num).c_str()

        #define p2 ("python3 ./downloader.py " + \
        mode + " " + to_string(unfirst) + " " + global_vars::tid + " " + pid_or_num + " " + num).c_str()
        #endif

        if (mode == "content"){
            system(p);
            cout  << "[" << time(0) << "]  " << "c++_p:" << p << endl;
        }
        if(mode == "lzl" && unfirst == false){
            system(p);
            cout  << "[" << time(0) << "]  " << "c++_p:" << p << endl;
        }
        if(mode == "lzl_list"){
            system(p);
            cout  << "[" << time(0) << "]  " << "c++_p:" << p << endl;
        }
        if(mode == "lzl" && unfirst == true){
            system(p2);
            cout  << "[" << time(0) << "]  " << "c++_p2:" << p2 << endl;
        }
        

        #ifdef LIN
        if (mode == "content")
            return "./" + global_vars::name + "/source/";
        if (mode == "lzl"||mode == "lzl_list")
            return "./" + global_vars::name + "/lzl/";
        else
            return "error!";
        #endif
        #ifdef WIN
        if (mode == "content")
            return ".\\" + global_vars::name + "\\source\\";
        if (mode == "lzl"||mode == "lzl_list")
            return ".\\" + global_vars::name + "\\lzl\\";
        else
            return "error!";
        #endif
    }
};