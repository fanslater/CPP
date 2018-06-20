/*! @CQueue.hpp
********************************************************************************
<PRE>
ģ����       : �����������߶���
�ļ���       : CQueue.hpp
����ļ�     : 
ʵ�ֹ���     : �����������߶���
����         : szluosj@21cn.com
�汾         : v1.0
--------------------------------------------------------------------------------
���̰߳�ȫ�� : ��
�쳣ʱ��ȫ�� : ��
--------------------------------------------------------------------------------
��ע         : 
--------------------------------------------------------------------------------
�޸ļ�¼ : 
�� ��        �汾     �޸���              �޸�����
2012/04/24   v1.0     luosj               ����
</PRE>
*******************************************************************************/

#ifndef __CPCQUEUE_H__
#define __CPCQUEUE_H__

//�����������߶���
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

  size_t m_nMaxSize; //�����д�С
  int m_nWaitCount;  //�ȴ�����

  //std::set<Data> m_setUnique;  
  bool m_bUnique; //�Ƿ���ҪΨһ
public:
  //---------------------------------
  CQueue(bool bUnique=false)
  {
    m_nMaxSize = 0; //ȱʡ������ 256
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
      {//����������ޣ���Ҫ�ȴ�
        //MyTools::OutputDebugStringEx("queue ���޵ȴ�:%u", the_queue.size());
        m_nWaitCount++;
        m_cvPut.wait(lock);
      }
      //MyTools::OutputDebugStringEx("queue ���޵ȴ�:%u ����", the_queue.size());
    }

    //�����ṹץϹ��
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

  Ȼ���壺std::set <Myline,comp> ctr(compare);
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
      else  if (nTimeoutMS< 0) //����
        the_condition_variable.wait(lock);  
      else// if (nTimeoutMS>0)
        if (!the_condition_variable.timed_wait(lock, boost::posix_time::milliseconds(nTimeoutMS)))
          return false;
    }  

    popped_value=the_queue.front();  
    the_queue.pop();

    //if (the_queue.size() == (m_nMaxSize-1))
    //    m_cvPut.notify_one(); //֪ͨ���Է���д�߳̿��Է���

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

