#include "StdAfx.h"
#include "XmlIniParser.h"

CXmlIniParser::CXmlIniParser( void )
{
    clean();
}

CXmlIniParser::CXmlIniParser( int filetype, const std::string& filepath )
{
    clean();
    try
    {
        loadfile( filetype, filepath );
    }
    catch( boost::property_tree::file_parser_error e )
    {
        showerror( e.what() );
    }
}

CXmlIniParser::~CXmlIniParser( void )
{
    clean();
}

void CXmlIniParser::clean()
{
    m_ptree.clear();
    m_bLoadFlag = true;
    m_file_type = 0;
    m_iLastErrCode = 0;
    m_strLastErrMsg.clear();
}

void CXmlIniParser::showerror( std::string msg )
{
#ifdef _CONSOLE
    printf( "%s\n", msg.c_str() );
#else if _WINDOWS
    TRACE( msg.c_str() );
#endif
}

void CXmlIniParser::loadfile( int filetype, const std::string& filepath )
{
    m_ptree.clear();
    try
    {
        if( filetype == ini_file )
        {
            boost::property_tree::ini_parser::read_ini( filepath, m_ptree );
            m_file_type = ini_file;
        }
        else if( filetype == xml_file )
        {
            boost::property_tree::xml_parser::read_xml( filepath, m_ptree, boost::property_tree::xml_parser::trim_whitespace );
            m_file_type = xml_file;
        }
    }
    catch( boost::property_tree::file_parser_error e )
    {
        m_bLoadFlag = false;
        m_iLastErrCode = -1;
        m_strLastErrMsg = e.what();
        showerror( e.what() );
        throw e;
    }
}

std::string CXmlIniParser::get( std::string path )
{
    if( m_bLoadFlag == false )
    {
        return "";
    }
    std::string value;
    try
    {
        value = m_ptree.get<std::string>( path );
    }
    catch( boost::property_tree::ptree_error e )
    {
        m_iLastErrCode = -3;
        m_strLastErrMsg = e.what();
        showerror( e.what() );
    }
    return value;
}

int CXmlIniParser::set( std::string path, std::string value )
{
    if( m_bLoadFlag == false )
    {
        return -1;
    }
    try
    {
        m_ptree.put( path, value );
    }
    catch( boost::property_tree::ptree_error e )
    {
        m_iLastErrCode = -4;
        m_strLastErrMsg = e.what();
        showerror( e.what() );
    }
    return 0;
}

int CXmlIniParser::add( std::string path, std::string value )
{
    if( m_bLoadFlag == false )
    {
        return -1;
    }
    try
    {
        m_ptree.add( path, value );
    }
    catch( boost::property_tree::ptree_error e )
    {
        m_iLastErrCode = -5;
        m_strLastErrMsg = e.what();
        showerror( e.what() );
    }
    return 0;
}

int CXmlIniParser::del( std::string path )
{
    if( m_bLoadFlag == false )
    {
        return -1;
    }
    try
    {
        int iPos = path.find_last_of( "." );
        if( iPos > 0 )
        {
            std::string url = path.substr( 0, iPos );
            std::string node = path.substr( iPos + 1 );
            boost::property_tree::ptree* pt = &( m_ptree.get_child( url ) );
            pt->erase( node );
        }
        else
        {
            m_ptree.erase( path );
        }
    }
    catch( boost::property_tree::ptree_error e )
    {
        m_iLastErrCode = -6;
        m_strLastErrMsg = e.what();
        showerror( e.what() );
    }
    return 0;
}

void CXmlIniParser::savefile( std::string filepath, int filetype )
{
    if( filetype == -1 )
    {
        filetype = m_file_type;
    }
    boost::property_tree::xml_writer_settings<std::string> settings( ' ', 2, "gb2312" );
    try
    {
        if( filetype == ini_file )
        {
            boost::property_tree::ini_parser::write_ini( filepath, m_ptree );
        }
        else if( filetype == xml_file )
        {
            boost::property_tree::xml_parser::write_xml( filepath, m_ptree, std::locale(), settings );
        }
    }
    catch( boost::property_tree::file_parser_error e )
    {
        m_iLastErrCode = -2;
        m_strLastErrMsg = e.what();
        showerror( e.what() );
    }
}

bool CXmlIniParser::is_load_success()
{
    return m_bLoadFlag;
}

void CXmlIniParser::get_last_err( int& iCode, std::string& strMsg )
{
    iCode = m_iLastErrCode;
    strMsg = m_strLastErrMsg;
}

boost::property_tree::ptree& CXmlIniParser::getTree( std::string path )
{
    if( path.length() == 0 )
    {
        return m_ptree;
    }
    return m_ptree.get_child( path );
}

int CXmlIniParser::setTree( boost::property_tree::ptree& pt )
{
    m_ptree = pt;
    return 0;
}

void CXmlIniParser::tree_traversal( boost::property_tree::ptree& root )
{
    BOOST_FOREACH( boost::property_tree::ptree::value_type & v1, root )
    {
        //如果当前节点为属性，打印全部属性信息
        if( v1.first == "<xmlattr>" )
        {
            BOOST_FOREACH( boost::property_tree::ptree::value_type & vAttr, v1.second )
            {
                std::cout << vAttr.first << "=" << vAttr.second.data() << " ";
            }
            printf( "\n" );
        }
        else
        {
            //如果没有子节点，则打印键值对
            if( v1.second.empty() )
            {
                std::cout << v1.first << "=" << v1.second.data() << std::endl;
            }
            else
            {
                //打印当前节点的名称
                std::cout << v1.first << std::endl;
                tree_traversal( v1.second );
                if( v1.second.size() == 0x01 )
                    std::cout << root.get<std::string>( v1.first ) << std::endl;
            }
        }
    }
}
