#pragma once
#include "ITask.h"
#include "IEncoder.h"
#include "../include/datapacket.h"

namespace robin
{
	class HttpEncode : public IEncoder
	{
		// �����ֱ�ӱ��뵽������Ȼ����
		virtual void encodeTask(std::vector<char>& buffer, TaskPtr task) override
		{


			std::string str =
			//"GET /test.json HTTP/1.1\r\n"
				"GET /data/sk/101010100.html HTTP/1.1\r\n"
				"Host:www.weather.com.cn\r\n"
				"User-Agent:Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:89.0) Gecko/20100101 Firefox/89.0\r\n"
				"Accept:text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n"
				"\r\n";


			for (int i=0; i<str.size(); i++)
			{
				buffer.push_back(str[i]);
			}
			// ������һ��0������Է�����
			buffer.push_back('\0');


		}
	};
}