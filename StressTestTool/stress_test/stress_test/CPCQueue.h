#pragma once
#include <queue>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include "BaseTool.h"

template<typename Data>
class CPCQueue
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
    CPCQueue(bool bUnique = false);
    ~CPCQueue(void);
public:
    int push(Data const &data);
    int clean();
    bool empty() const;
    int size() const;
    bool wait_and_pop(Data &popped_value, int nTimeoutMS = -1);
};

template<typename Data>
class CPCQueueHelper
{
private:
    CPCQueue<Data> *m_pQueue;
    Data m_value;
public:
    CPCQueueHelper(CPCQueue<Data> *pQueue, Data &popped_value);
    ~CPCQueueHelper();
};


template<typename Data>
CPCQueue<Data>::CPCQueue(bool bUnique/*=false*/)
{
    m_nMaxSize = 0; //缺省最大深度 256
    m_nWaitCount = 0;
    //m_bUnique = bUnique;
}

template<typename Data>
CPCQueue<Data>::~CPCQueue(void)
{
    clean();
}

template<typename Data>
int CPCQueue<Data>::push(Data const &data)
{
    size_t nSize = 0;
    boost::mutex::scoped_lock lock(the_mutex);

    if (m_nMaxSize > 0)
    {
        if  (the_queue.size() >= m_nMaxSize)
        {
            //超过最大上限，需要等待
            CBaseTool::OutputDebugStringEx("queue 超限等待:%u", the_queue.size());
            m_nWaitCount++;
            m_cvPut.wait(lock);
        }
        CBaseTool::OutputDebugStringEx("queue 超限等待:%u 解锁", the_queue.size());
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
    if (nSize > nSizeMax)
    {
        nSizeMax = nSize;
        if (nSize >= 1000)
            CBaseTool::OutputDebugStringEx((char *)"queue size:%u", nSize);
    }

    return nSize;
}

template<typename Data>
int CPCQueue<Data>::clean()
{
    boost::mutex::scoped_lock lock(the_mutex);
    while(!the_queue.empty())
    {
        the_queue.pop();
    }
    return 0;
}

template<typename Data>
bool CPCQueue<Data>::empty() const
{
    boost::mutex::scoped_lock lock(the_mutex);
    return the_queue.empty();
}

template<typename Data>
int CPCQueue<Data>::size() const
{
    boost::mutex::scoped_lock lock(the_mutex);
    return the_queue.size();
}

template<typename Data>
bool CPCQueue<Data>::wait_and_pop(Data &popped_value, int nTimeoutMS/*=-1*/)
{
    /*
            if (nTimeoutMS==0)
                return try_pop(popped_value);
    */
    boost::mutex::scoped_lock lock(the_mutex);
    while(the_queue.empty())
    {
        if (nTimeoutMS == 0)
            return false;
        else  if (nTimeoutMS < 0) //死等
            the_condition_variable.wait(lock);
        else// if (nTimeoutMS>0)
            if (!the_condition_variable.timed_wait(lock, boost::posix_time::milliseconds(nTimeoutMS)))
                return false;
    }

    popped_value = the_queue.front();
    the_queue.pop();

    //if (the_queue.size() == (m_nMaxSize-1))
    //    m_cvPut.notify_one(); //通知可以放了写线程可以放了

    if (m_nMaxSize > 0)
    {
        if (the_queue.size() < m_nMaxSize)
        {
            if (m_nWaitCount > 0)
            {
                m_cvPut.notify_one();
                m_nWaitCount--;
            }
        }
    }
    return true;
}

template<typename Data>
CPCQueueHelper<Data>::CPCQueueHelper(CPCQueue<Data> *pQueue, Data &popped_value)
{
    if (pQueue == NULL)
        throw std::logic_error("Can't find this MCU Resource pool!");

    m_pQueue = pQueue;
    m_pQueue->wait_and_pop(popped_value);
    m_value = popped_value;
}

template<typename Data>
CPCQueueHelper<Data>::~CPCQueueHelper()
{
    m_pQueue->push(m_value);
}