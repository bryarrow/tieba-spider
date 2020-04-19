#pragma once

#include <regex>

using namespace std;
namespace regs{
    regex content("j_d_post_content \" style=\"display:;\">(.*?)</div>");                                // 匹配帖子正文
    regex content_h("j_d_post_content \" style=\"display:;\">*");                                      // 匹配正文前缀
    regex content_e("</div>");                                                                         // 匹配正文后缀
    regex pop_content("<div class=\"post_bubble_middle_inner\">(.*?)</div>");                            // 匹配气泡内正文
    regex pop_h("<div class=\"post_bubble_middle_inner\">");                                           // 匹配气泡内正文前缀
    #define pop_e content_e                                                                            // 匹配气泡内正文后缀（因为和正文后缀一样就用了宏）
    regex pid("<div id=\"post_content_.*?>");                                                          // 匹配pid，用于爬取评论
    regex pid_h("<div id=\"post_content_");                                                            // 匹配pid前缀
    regex pid_e("\" class=\"d_post_content j_d_post_content \" style=\"display:;\">");                 // 匹配pid后缀
    regex flnum("<span class=\"tail-info\">([0-9].*?)</span>");                                          // 匹配楼层数
    regex flnum_h("<span class=\"tail-info\">");                                                       // 匹配楼层数前缀
    regex flnum_e("</span>");                                                                          // 匹配楼层数后缀
    regex fltime("[0-9][0-9][0-9][0-9]-[0-9][0-9]-[0-9][0-9] [0-9][0-9]:[0-9][0-9]");                  // 匹配发帖时间
    regex rnum("([0-9]*)\">尾页");                                                                       // 匹配总页数
    regex rnum_e("\">尾页");                                                                           // 匹配总页数后缀
    regex br("<br>");                                                                                  // 替换<br>标签
    regex linehand("            ");                                                                    // 用于替换行首空格
    regex username("&fr=pb\" target=\"_blank\">(.*)</a>");                                               // 匹配昵称
    regex usrname_h("&fr=pb\" target=\"_blank\">");                                                    // 匹配昵称前缀
    regex usrname_e("</a>");                                                                           // 匹配昵称后缀
    regex lzl("target=\"_blank\" username=\".*?\">.*?</span><div class=\"lzl_content_reply\">");
    regex lzl_username("target=\"_blank\" username=\".*?\">(.*?)</a>:<span class=\"lzl_content_main\""); // 匹配楼中楼昵称
    regex lzl_uname_h("target=\"_blank\" username=\".*?\">");                                          // 匹配楼中楼昵称前缀
    regex lzl_uname_e("</a>:<span class=\"lzl_content_main\"");                                        // 匹配楼中楼昵称后缀
    regex reply("target=\"_blank\" class=\"at\">(.*?)</a>");
    regex reply_h("target=\"_blank\" class=\"at\">");
    regex reply_e("</a>");
    regex con_rp("</a> :(.*?)</span>");
    regex con_rp_h("</a> :");
    regex con_nrp("class=\"lzl_content_main\" data-username=\"\">       (.*?)</span>");
    regex con_e("</span>");
    regex huifu("回复 <.*?>(.*)<\\/a> :");
    smatch res;                                                                                        // 保存匹配结果
}