/**
 * Tencent is pleased to support the open source community by making Tars available.
 *
 * Copyright (C) 2016THL A29 Limited, a Tencent company. All rights reserved.
 *
 * Licensed under the BSD 3-Clause License (the "License"); you may not use this file except 
 * in compliance with the License. You may obtain a copy of the License at
 *
 * https://opensource.org/licenses/BSD-3-Clause
 *
 * Unless required by applicable law or agreed to in writing, software distributed 
 * under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR 
 * CONDITIONS OF ANY KIND, either express or implied. See the License for the 
 * specific language governing permissions and limitations under the License.
 */

/*

TC_ThreadThreadControl_Exception : public TC_Exception
TC_ThreadThreadControl_Exception(const string &buffer) : TC_Exception(buffer){}
TC_ThreadThreadControl_Exception(const string &buffer, int err) : TC_Exception(buffer, err){}
~TC_ThreadThreadControl_Exception() throw() {}
TC_ThreadControl
通过线程号控制线程的类
TC_Runable
TC_Thread : public TC_Runable
线程主类
public:
TC_Thread();
~TC_Thread(){}
start();//新建线程在这里
getThreadControl() const;//返回对应的线程控制类
isAlive() const;
id() { return _tid; }
protected:
threadEntry(TC_Thread *pThread);// 线程入口函数
run() = 0;
protected:
_running;
_tid;
_lock;
#endif

#ifndef __TC_THREAD_H_
	#define __TC_THREAD_H_
	#includes
	tars
		TC_ThreadThreadControl_Exception : public TC_Exception
			TC_ThreadThreadControl_Exception(const string &buffer) : TC_Exception(buffer){}
			TC_ThreadThreadControl_Exception(const string &buffer, int err) : TC_Exception(buffer, err){}
			~TC_ThreadThreadControl_Exception() throw() {}
		TC_ThreadControl
		TC_Runable
		TC_Thread : public TC_Runable
			public:
				TC_Thread();
				~TC_Thread(){}
				start();
				getThreadControl() const;
				isAlive() const;
				id() { return _tid; }
			protected:
				threadEntry(TC_Thread *pThread);
				run() = 0;
			protected:
				_running;
				_tid;
				_lock;
	#endif

*/

#ifndef __TC_THREAD_H_
#define __TC_THREAD_H_

#include <sys/types.h>
#include <signal.h>
#include <pthread.h>
#include "util/tc_ex.h"
#include "util/tc_monitor.h"

namespace tars
{
/////////////////////////////////////////////////
/** 
 * @file tc_thread.h 
 * 
 */          
/////////////////////////////////////////////////

/**
 * @brief  线程控制异常类
 */
struct TC_ThreadThreadControl_Exception : public TC_Exception
{
    TC_ThreadThreadControl_Exception(const string &buffer) : TC_Exception(buffer){};
    TC_ThreadThreadControl_Exception(const string &buffer, int err) : TC_Exception(buffer, err){};
    ~TC_ThreadThreadControl_Exception() throw() {};
};

/**
 * @brief  线程控制类
 */
class TC_ThreadControl
{
public:

    /**
     * @brief  构造, 表示当前运行的线程，
     *          join和detach在不能在该对象上调用
    */
       
    TC_ThreadControl();

    /**
	 * @brief  通过线程的pid来实现对线程的管理
     * @return explicit
     */
    explicit TC_ThreadControl(pthread_t);

    /**
     * @brief  等待当前线程结束, 不能在当前线程上调用
     */
    void join();

    /**
     * @brief  detach, 不能在当前线程上调用
     */
    void detach();

    /**
     * @brief  获取当前线程id.
     *
     * @return pthread_t当前线程id
     */
    pthread_t id() const;

    /**
     * @brief  休息ms时间. 
     *  
     * @param millsecond 休息的时间，具体ms数字
     */
    static void sleep(long millsecond);

    /**
     * @brief  交出当前线程控制权
     */
    static void yield();

private:

    pthread_t _thread;
};

/**
 *
 */
class TC_Runable
{
public:
    virtual ~TC_Runable(){};
    virtual void run() = 0;
};

/**
 * @brief 线程基类. 
 * 线程基类，所有自定义线程继承于该类，同时实现run接口即可, 
 *  
 * 可以通过TC_ThreadContorl管理线程。
 */
class TC_Thread : public TC_Runable
{
public:

    /**
     * @brief  构造函数
     */
    TC_Thread();

    /**
     * @brief  析构函数
     */
    virtual ~TC_Thread(){};

    /**
     * @brief  线程运行
     */
    TC_ThreadControl start();

    /**
     * @brief  获取线程控制类.
     *
     * @return ThreadControl
     */
    TC_ThreadControl getThreadControl() const;

    /**
     * @brief  线程是否存活.
     *
     * @return bool 存活返回true，否则返回false
     */
    bool isAlive() const;

    /**
     * @brief  获取线程id.
     *
     * @return pthread_t 线程id
     */
    pthread_t id() { return _tid; }

protected:

    /**
     * @brief  静态函数, 线程入口. 
	 * 该函数要传给pthread_create来创建线程：
	 * int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
						  void *(*start_routine) (void *), void *arg);
	 * 由于pthread_create的start_routine形参只接受参数为 void*的函数
	 * 而一般类的非静态成员函数的第一个参数都是*this，所以没法作为参数传入
	 * 故这里把threadEntry设计成静态成员函数。由于所有类公用这一个函数，所以要把调用它的线程类传进去
	   https://blog.csdn.net/boshuzhang/article/details/52275710
     * @param pThread 线程对象
     */
    static void threadEntry(TC_Thread *pThread);

    /**
     * @brief  运行
	 这里run只是一个纯虚函数，并未给出其实现
     */
    virtual void run() = 0;

protected:
    /**
     * 是否在运行
     */
    bool            _running;

    /**
     * 线程ID
     */
    pthread_t        _tid;

    /**
     * 线程锁
     */
    TC_ThreadLock   _lock;
};

}
#endif

