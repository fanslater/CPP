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




class CXmlIniReader
{
public:
    CXmlIniReader();
    CXmlIniReader(int filetype, std::string filepath);
    ~CXmlIniReader();

private:
    int m_err_code;
    int m_file_type;
    boost::property_tree::ptree m_ptree;

private:
    void showerror(std::string msg);

public:
    void loadfile(int filetype, std::string filepath);
    void savefile(std::string filepath, int filetype = -1);

    std::string get(std::string path);
    int set(std::string path, std::string value);	  //���򴴽������򸲸ǡ����Դ���·���ϵ����и��ڵ㡣
    int add(std::string path, std::string value);      //���򴴽��������ٽ������Դ���·���ϵ����и��ڵ㡣
    int del(std::string path);                         //ɾ����·�����²�ڵ���������ݡ�

    boost::property_tree::ptree &getTree(std::string path = "");
    int setTree(boost::property_tree::ptree &pt);

    int get_last_errcode();
    void tree_traversal(boost::property_tree::ptree &root);		//������
};
