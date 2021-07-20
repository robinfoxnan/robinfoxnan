#pragma once
#include <chrono>
#include <string>
#include <time.h>
#include <time.h>
#include <ctime>

#include <iostream>
#include <iomanip>      // std::put_time
#include <sstream>
#include <sys/timeb.h>

namespace robin
{
	// ��ʽ��ʱ���ַ���
	/*
	localtime_rҲ��������ȡϵͳʱ�䣬������linuxƽ̨��
	����ԭ��Ϊstruct tm *localtime_r(const time_t *timep, struct tm *result);
	localtime_sҲ��������ȡϵͳʱ�䣬������windowsƽ̨�£���localtime_rֻ�в���˳��һ��
	*/
	std::string getTimeNow()
	{
		//printf("----%s----%d---\n", __FUNCTION__, __LINE__);
		
		auto now = std::chrono::system_clock::now();
		time_t tt = std::chrono::system_clock::to_time_t(now);
		struct tm tm;

#if defined(WIN32) || defined(_WIN32)
		localtime_s(&tm, &tt);  // ת��tm�ṹ
#elif  defined(__linux) || defined(__linux__) 
		localtime_r(&tt, &tm);	
#endif	
		int minsecs = tt & 1000;
		std::ostringstream stream;
		stream << std::put_time(&tm, "%F %T");
		stream << ".";
		stream << minsecs;
		return stream.str();
	}

	std::time_t getTimeStamp()
	{
		std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> tp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
		auto tmp = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch());
		std::time_t timestamp = tmp.count();
		//std::time_t timestamp = std::chrono::system_clock::to_time_t(tp);
		return timestamp;
	}

	void getTimeStamp(uint64_t & sec, uint32_t & msec)
	{
		timeb now;
		ftime(&now);
		sec = now.time;
		msec = now.millitm;
		//return now.time * 1000 + now.millitm;
	}


	/*
	 * ����ʱ��ļ����⺯���鿴�ֲ��֪asctime(), ctime(), gmtime()�� localtime()���ǲ���ȫ�ģ�
	 * ��Ϊ�⼸����������һ��ָ�룬���ָ��ָ��һ�ξ�̬�ڴ������������̲߳���ȫ�ģ�
	 * ���ĸ��������̰߳�ȫ�汾��asctime_r(), ctime_r(),gmtime_r()�� localtime_r()
	*/

	/*std::string getTimeNowold()
	{
		time_t rawtime;
		struct tm * timeinfo;
		char buffer[80];

		time(&rawtime);
		timeinfo = localtime(&rawtime);

		strftime(buffer, 80, "Now it's %I:%M%p.", timeinfo);
		return buffer;
	}
*/
}