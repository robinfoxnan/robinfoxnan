#pragma once
#include "SimpleMsgDispatcher.h"

namespace robin
{

class JsonMsgDispatcher :public SimpleMsgDispatcher
{
public:
	// ����������ʵ��
	virtual void onMessageParse(DATA_HEADER * header, char *buf, unsigned long len, TcpConnection *conn) override;
};
}
