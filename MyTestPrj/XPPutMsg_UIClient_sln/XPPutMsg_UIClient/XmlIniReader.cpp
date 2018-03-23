#include "StdAfx.h"
#include "XmlIniReader.h"

CXmlIniReader::CXmlIniReader(void)
{
	m_err_code = 0;
}

CXmlIniReader::CXmlIniReader(int filetype, string filepath)
{
	m_err_code = 0;
	loadfile(filetype,filepath);
}

CXmlIniReader::~CXmlIniReader(void)
{
}

void CXmlIniReader::showerror(string msg)
{
#ifdef _CONSOLE 
	printf("%s\n",msg.c_str()); 
#else if _WINDOWS
	TRACE(msg.c_str());
#endif
}

void CXmlIniReader::loadfile(int filetype, string filepath)
{
	try
	{
		if (filetype == ini_file)
		{
			boost::property_tree::ini_parser::read_ini(filepath, m_ptree); 
			m_file_type = ini_file;
		} 
		else if(filetype == xml_file)
		{
			boost::property_tree::xml_parser::read_xml(filepath,m_ptree);
			m_file_type = xml_file;
		}
	}
	catch (boost::property_tree::file_parser_error e)
	{
		m_err_code = -1;
		showerror(e.what());
	}
}

std::string CXmlIniReader::get(string path)
{
	if (m_err_code != 0)
	{
		return "";
	}

	string value;
	try
	{
		value = m_ptree.get<string>(path);
	}
	catch(boost::property_tree::ptree_error e)
	{
		m_err_code = -1;
		showerror(e.what());
	}
	return value;
}

int CXmlIniReader::set(string path,string value)
{
	if (m_err_code != 0)
	{
		return m_err_code;
	}

	try
	{
		m_ptree.put(path,value);
	}
	catch(boost::property_tree::ptree_error e)
	{
		m_err_code = -1;
		showerror(e.what());
	}

	return 0;
}

int CXmlIniReader::add(string path,string value)
{
	if (m_err_code != 0)
	{
		return m_err_code;
	}

	try
	{
		m_ptree.add(path,value);
	}
	catch(boost::property_tree::ptree_error e)
	{
		m_err_code = -1;
		showerror(e.what());
	}

	return 0;
}

void CXmlIniReader::savefile(string filepath)
{
	try
	{
		if (m_file_type == ini_file)
		{
			boost::property_tree::ini_parser::write_ini(filepath,m_ptree);
		}
		else if(m_file_type == xml_file)
		{
			boost::property_tree::xml_parser::write_xml(filepath,m_ptree);
		}		
	}
	catch(boost::property_tree::file_parser_error e)
	{
		showerror(e.what());
	}
}

int CXmlIniReader::get_last_errcode()
{	
	return m_err_code;
}

boost::property_tree::ptree& CXmlIniReader::getTree()
{
	return m_ptree;
}

void CXmlIniReader::tree_traversal(boost::property_tree::ptree& root)
{
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v1, root)  
	{  
		//如果当前节点为属性，打印全部属性信息  
		if(v1.first == "<xmlattr>")  
		{  
			BOOST_FOREACH(boost::property_tree::ptree::value_type &vAttr, v1.second)  
			{  
				std::cout<<vAttr.first<<"="<<vAttr.second.data()<<" ";  
			}  
			printf("\n");
		}  
		else  
		{  
			//如果没有子节点，则打印键值对  
			if (v1.second.empty())  
			{  
				std::cout<<v1.first<<"="<<v1.second.data()<<std::endl;  
			}  
			else   
			{  
				//打印当前节点的名称  
				std::cout<<v1.first<<std::endl;  
				tree_traversal(v1.second);  
				if(v1.second.size() == 0x01)  
					std::cout<<root.get<std::string>(v1.first)<<std::endl;  
			}  
		}  
	} 
}
