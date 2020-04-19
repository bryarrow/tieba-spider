#pragma once

#include <fstream>
#include <string>

namespace global_vars{
    std::string path;
    std::string path_python;
    std::string tid;
    std::string name;
    std::ifstream in;   //HTML源码文件流
    std::ofstream out;  //输出文件流
    std::string linein; //当前操作的那一行
    std::string l_pn;   //处理本地文件时页数
    bool is_local; //是否仅处理本地文件
}


