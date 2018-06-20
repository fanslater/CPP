#ifndef __Public_h__
#define __Public_h__

#define IOCP_WriteLog(str) //LOG4CPLUS_INFO(log4cplus::Logger::getInstance(""), str)
#define IOCP_ErrorLog(str) //LOG4CPLUS_ERROR(log4cplus::Logger::getInstance(""), str)
#define IOCP_WarnLog(str) //LOG4CPLUS_WARN(log4cplus::Logger::getInstance(""), str)

#define SharedPtr boost::shared_ptr

#define DeclearMutex(MutexName) mutable boost::recursive_mutex m_##MutexName
#define AutoLock(MutexName) boost::recursive_mutex::scoped_lock lock(m_##MutexName)

#endif
