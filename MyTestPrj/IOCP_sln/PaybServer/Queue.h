/*! @CQueue.hpp
********************************************************************************
<PRE>
模块名       : 生产者消费者队列
文件名       : CQueue.hpp
相关文件     : 
实现功能     : 生产者消费者队列
作者         : szluosj@21cn.com
版本         : v1.0
--------------------------------------------------------------------------------
多线程安全性 : 是
异常时安全性 : 是
--------------------------------------------------------------------------------
备注         : 
--------------------------------------------------------------------------------
修改记录 : 
日 期        版本     修改人              修改内容
2012/04/24   v1.0     luosj               创建
</PRE>
*******************************************************************************/

#ifndef __CPCQUEUE_H__
#define __CPCQUEUE_H__

//生产者消费者队列
#include <queue>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include<set>
#include "Public.h"

template<typename Data>  
class CQueue  
{  
private:  
  std::queue<Data> the_queue;  
  mutable boost::mutex the_mutex;  
  boost::condition_variable the_condition_variable;
  boost::condition_variable m_cvPut;

  size_t m_nMaxSize; //最大队列大小
  int m_nWaitCount;  //等待个数

  //std::set<Data> m_setUnique;  
  bool m_bUnique; //是否需要唯一
public:
  //---------------------------------
  CQueue(bool bUnique=false)
  {
    m_nMaxSize = 0; //缺省最大深度 256
    m_nWaitCount = 0;
    //m_bUnique = bUnique;
  }
  //---------------------------------
  int push(Data const& data)
  {
    size_t nSize = 0;
    boost::mutex::scoped_lock lock(the_mutex);  

    if (m_nMaxSize>0)
    {
      if  (the_queue.size() >= m_nMaxSize)
      {//超过最大上限，需要等待
        //MyTools::OutputDebugStringEx("queue 超限等待:%u", the_queue.size());
        m_nWaitCount++;
        m_cvPut.wait(lock);
      }
      //MyTools::OutputDebugStringEx("queue 超限等待:%u 解锁", the_queue.size());
    }

    //遇到结构抓瞎了
    //if (m_bUnique)
    //{
    //m_setUnique.insert(data);
    //}

    the_queue.push(data);  
    nSize = the_queue.size();
    lock.unlock();  
    the_condition_variable.notify_one();  

    static size_t nSizeMax = 0;
    if (nSize>nSizeMax)
    {
      nSizeMax = nSize;
      if (nSize>=1000)
      {
        tstringstream ss;
        ss << "queue size:" << nSize;
        IOCP_WriteLog( ss.str() );
      }
    }

    return nSize;
  }  
  //---------------------------------
  bool empty() const 
  {  
    boost::mutex::scoped_lock lock(the_mutex);  
    return the_queue.empty();  
  }  
  //---------------------------------

  int size() const 
  {  
    boost::mutex::scoped_lock lock(the_mutex);  
    return the_queue.size(); 
  }  
  /*
  bool try_pop(Data& popped_value)  
  {  
  boost::mutex::scoped_lock lock(the_mutex);  
  if(the_queue.empty())  
  {  
  return false;  
  }  

  popped_value=the_queue.front();  
  the_queue.pop();  
  return true;  
  }  

  typedef bool (*comp)(const Myline &, const Myline &)
  bool compare() (const Myline &a, const Myline &b) 
  { 
  if (a.L<b.L)return true;

  else if(a.L==b.L&&a.R<b.R)return true;

  else return false; 

  }

  然后定义：std::set <Myline,comp> ctr(compare);
  */
  //---------------------------------

  bool wait_and_pop(Data& popped_value, int nTimeoutMS=-1)
  {  
    /*
    if (nTimeoutMS==0)
    return try_pop(popped_value);
    */
    boost::mutex::scoped_lock lock(the_mutex);  
    while(the_queue.empty())  
    {  
      if (nTimeoutMS==0)
        return false;
      else  if (nTimeoutMS< 0) //死等
        the_condition_variable.wait(lock);  
      else// if (nTimeoutMS>0)
        if (!the_condition_variable.timed_wait(lock, boost::posix_time::milliseconds(nTimeoutMS)))
          return false;
    }  

    popped_value=the_queue.front();  
    the_queue.pop();

    //if (the_queue.size() == (m_nMaxSize-1))
    //    m_cvPut.notify_one(); //通知可以放了写线程可以放了

    if (m_nMaxSize>0)
    {
      if (the_queue.size() < m_nMaxSize)
      {
        if (m_nWaitCount>0)
        {
          m_cvPut.notify_one();
          m_nWaitCount--;
        }
      }
    }
    return true;
  }  
};

#endif// #ifndef __CPCQUEUE_H__

