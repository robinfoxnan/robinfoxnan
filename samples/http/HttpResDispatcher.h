#pragma once
#include <iostream>
#include "IDispatcher.h"

namespace robin
{

	class HttpResDispatcher : public IDispatcher
	{
	public:
		virtual void onMessage(void * client, char *buf, unsigned long len) override
		{
			std::string str(buf, len);
			std::cout << "�յ�Ӧ��" << str << std::endl;
			std::cout << "\n\n";

		}
	};
}

