#ifndef PARSE_H
#define PARSE_H
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <map>
#include <utility>
#include <sstream>
#include <ctype.h>
#include <iostream>


typedef std::map<std::string, std::string> 	tyhp_header;

//#define string::npos			REQUEST_END
#define make_tyhp_header(key, value)	\
make_pair((key), (value))

    //保存从http request解析下来的值
    typedef struct _tyhp_http_header_t
{
    std::string 		method;
    std::string 		url;
    std::string		version;

    tyhp_header header;

    std::string 	body;
}tyhp_http_header_t;

/*
 *函数作用：打印tyhp_http_header_t里的header
 *函数参数：tyhp_header 的const 引用
 *函数返回值: 无
 */
void tyhp_print_http_header_header(const tyhp_header& head);
/*
 *函数作用：打印tyhp_http_header_t
 *函数参数：tyhp_http_header_t指针
 *函数返回值: 无
 */
void tyhp_print_http_header(tyhp_http_header_t *phttphdr);

/*
 *函数作用：分配内存给tyhp_http_header_t
 *函数参数：无
 *函数返回值: NULL表示分配失败，其他值表示成功
 */
tyhp_http_header_t *tyhp_alloc_http_header();
/*
 *函数作用：回收分配给tyhp_http_header_t的内存
 *函数参数：tyhp_http_header_t指针
 *函数返回值: 无
 */
void tyhp_free_http_header(tyhp_http_header_t *phttphdr);
/*
 *函数作用：解析http_request
 *函数参数：http_request为待解析的值，phttphdr保存了解析下来的值
 *函数返回值: true表示解析成功，false表示解析失败
 */
bool tyhp_parse_http_request(const std::string& http_request, tyhp_http_header_t *phttphdr);
/*
 *函数作用：根据key的值在phttphdr所指向的tyhp_http_header_t中查找相对应的值
 *函数参数：key为关键字，header
 *函数返回值: -返回空值表示查找失败，否则返回相应的值
 */
std::string tyhp_get_value_from_http_header(const std::string& key, const tyhp_header& header);
#endif // PARSE_H
