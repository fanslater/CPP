#pragma once

#include <boost/property_tree/ptree.hpp> 
#include <boost/property_tree/xml_parser.hpp>  
#include <boost/property_tree/ini_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp> 
#include <iostream>
#include <string>

using namespace std; 

enum file_type
{
	ini_file = 1,
	xml_file
};




class CXmlIniReader
{
public:
	CXmlIniReader();
	CXmlIniReader(int filetype, string filepath);
	~CXmlIniReader();

private:
	int m_err_code;
	int m_file_type;
	boost::property_tree::ptree m_ptree;

private:
	void showerror(string msg);

public:
	void loadfile(int filetype, string filepath);
	void savefile(string filepath);

	string get(string path);
	int set(string path,string value);	
	int add(string path,string value);
	
	int get_last_errcode();
	boost::property_tree::ptree& getTree();
	void tree_traversal(boost::property_tree::ptree& root);		//½áµã±éÀú
};
