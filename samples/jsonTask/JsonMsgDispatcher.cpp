#include "JsonMsgDispatcher.h"
#include <string>
#include <iostream>
#include "WorkerPool.h"
#include "jsonDecode.h"

namespace robin
{
	// ������Ҫ����conn������task��������������Ͳ�֪�����ݸûش���˭�ˣ�
	void JsonMsgDispatcher::onMessageParse(DATA_HEADER * header, char *buf, unsigned long len, TcpConnection *conn)
	{
		std::string str(buf, len);
		std::cout << str << std::endl;

		// ��������
		std::shared_ptr<ITask> task = JsonDecode::instance()->decodeJson(header, buf, len);
		if (task->errorCode != 0)  // ����ʧ��
		{

		}
		else
		{
			task->setConnection(conn);

			WorkerPool::addToWorkQueue(task);
		}
		
	}
}