#pragma once

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <string>

enum file_type
{
    ini_file = 1,
    xml_file
};

class CXmlIniParser
{
public:
    CXmlIniParser();
    CXmlIniParser( int filetype, const std::string& filepath );
    ~CXmlIniParser();

private:
    bool m_bLoadFlag;
    int m_iLastErrCode;
    std::string m_strLastErrMsg;
    int m_file_type;
    boost::property_tree::ptree m_ptree;

private:
    void clean();
    void showerror( std::string msg );

public:
    void loadfile( int filetype, const std::string& filepath );
    void savefile( std::string filepath, int filetype = -1 );
    bool is_load_success();
    void get_last_err( int& iCode, std::string& strMsg );

    std::string get( std::string path );
    int set( std::string path, std::string value );   //无则创建，有则覆盖。可以创建路径上的所有父节点。
    int add( std::string path, std::string value );    //无则创建，有则再建。可以创建路径上的所有父节点。
    int del( std::string path );                       //删除该路径最下层节点的整个内容。
    boost::property_tree::ptree& getTree( std::string path = "" );
    int setTree( boost::property_tree::ptree& pt );

    void tree_traversal( boost::property_tree::ptree& root );   //结点遍历
};
