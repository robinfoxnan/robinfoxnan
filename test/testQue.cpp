
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>
#include <chrono>
#include <ratio>
#include <deque>
#include <atomic>
#include <mutex>


#include <uv.h>
#include "../include/queue.h"
#include "../include/MyTimer.h"
#include "../include/spin_lock.h"


struct Student {
	Student(int a, const char * s) :age(a), name(s)
	{
	}

	Student() = default;
	QUEUE node;
	int age;
	std::string name;

};

class StudentAuto : public enable_shared_from_this<StudentAuto>
{
public:
	StudentAuto(int a, const char * s) :age(a), name(s)
	{
	}

	StudentAuto() = default;

	std::shared_ptr<StudentAuto> getptr() {
		return shared_from_this();
	}

	~StudentAuto()
	{
		//std::cout << "����StudentAuto" << endl;
	}
	QUEUE node;
	int age;
	std::string name;

};

void test4()
{
	// ��UV������Ϣ

	Timer timer1;
	timer1.start();

	QUEUE queue;
	QUEUE_INIT(&queue);

	for (int i = 0; i < 10000; i++)
	{
		std::shared_ptr<StudentAuto> it = make_shared<StudentAuto>(i + 1, "robin");

		QUEUE_INIT(&(it->node));
		QUEUE_INSERT_TAIL(&queue, &(it->node));
	}

	auto duration1 = timer1.stop_delta<std::milli>();
	std::cout << "����10000���ܶ���UV����(ms)��" << duration1 << endl;
}

// ϵͳæ
//����UV����(ms)��0.1168
//ͷ������(ms)�� 0.1078
//����std::deque(ms)�� 20.5516
//����std::deque(ms)�� 32.1022

// ����
//����UV����(ms)��0.045
//ͷ������(ms)�� 0.0611
//����std::deque(ms)�� 7.4969
//����std::deque(ms)�� 7.3878
void test1()
{
	Student * students = new Student[10000];
	Student * it = students;
	for (int i = 0; i < 10000; i++)
	{
		it = students + i;
		it->age = i + 1;
		it->name = "robin";
	}

	Timer timer1;
	timer1.start();

	// ��UV������Ϣ
	QUEUE queue;
	QUEUE_INIT(&queue);

	for (int i = 0; i < 10000; i++)
	{
		it = students + i;

		QUEUE_INIT(&(it->node));
		QUEUE_INSERT_TAIL(&queue, &(it->node));
	}



	auto duration1 = timer1.stop_delta<std::milli>();
	std::cout << "����UV����(ms)��" << duration1 << endl;

	// ͷ������
	QUEUE *p;
	timer1.start();
	for (int i = 0; i < 10000; i++)
	{
		p = QUEUE_HEAD(&queue);
		if (p != &queue)
		{
			QUEUE_REMOVE(p);
		}
	}
	duration1 = timer1.stop_delta<std::milli>();
	std::cout << "ͷ������(ms)�� " << duration1 << endl;

	// ����std::deque
	timer1.start();
	std::deque<Student *> studentQue;

	for (int i = 0; i < 10000; i++)
	{
		it = students + i;
		studentQue.push_back(it);
	}

	duration1 = timer1.stop_delta<std::milli>();
	std::cout << "����std::deque (ms)�� " << duration1 << endl;

	timer1.start();
	for (int i = 0; i < 10000; i++)
	{
		studentQue.pop_front();
	}
	duration1 = timer1.stop_delta<std::milli>();
	std::cout << "����std::deque (ms)�� " << duration1 << endl;

	// ȡ��ͷ����
	/*QUEUE *p;
	p = QUEUE_HEAD(&queue);
	p = QUEUE_NEXT(p);
	Student *first_stu = QUEUE_DATA(p, struct Student, node);*/

	/**
	* Should output the name of wesley.
	*/
	/*printf("Received first inserted Student: %s who is %d.\n",
		first_stu->name.c_str(), first_stu->age);
*/
/*QUEUE_FOREACH(p, &queue) {
	Student *tmp = QUEUE_DATA(p, struct Student, node);
	cout << "name: " << tmp->name << " age: " << tmp->age << endl;

}*/

	delete[] students;
}
// ϵͳæ
//����UV����(ms)��17.3848
//����std::deque(ms)�� 50.7232
//����std::deque(ms)�� 46.7094

// ϵͳ����
//����UV����(ms)��9.0955
//����std::deque(ms)�� 19.8008
//����std::deque(ms)�� 14.6486
void test2()
{

	// ��UV������Ϣ

	Timer timer1;
	timer1.start();

	Student * students = new Student[10000];
	Student * it = students;
	for (int i = 0; i < 10000; i++)
	{
		it = students + i;
		it->age = i + 1;
		it->name = "robin";
	}
	QUEUE queue;
	QUEUE_INIT(&queue);

	for (int i = 0; i < 10000; i++)
	{
		it = students + i;

		QUEUE_INIT(&(it->node));
		QUEUE_INSERT_TAIL(&queue, &(it->node));
	}

	auto duration1 = timer1.stop_delta<std::milli>();
	std::cout << "����UV����(ms)��" << duration1 << endl;
	/////////////////////////////////////////////////////////
	timer1.start();
	std::deque<Student> studentQue;
	for (int i = 0; i < 10000; i++)
	{
		studentQue.emplace_back(i + 1, "robin");
	}

	duration1 = timer1.stop_delta<std::milli>();
	std::cout << "����std::deque (ms)�� " << duration1 << endl;

	timer1.start();
	for (int i = 0; i < 10000; i++)
	{
		//cout << studentQue[0].age << endl;
		studentQue.pop_front();
	}
	duration1 = timer1.stop_delta<std::milli>();
	std::cout << "����std::deque (ms)�� " << duration1 << endl;

	delete[] students;
}

//����10000std::deque����ָ��(ms)�� 64.094
//����10000std::deque����(ms)�� 55.6962
using StudentPtr = std::shared_ptr<Student>;
void test3()
{

	Timer timer1;
	timer1.start();

	std::deque<StudentPtr> studentQ;
	for (int i = 0; i < 10000; i++)
	{
		studentQ.push_back(std::make_shared<Student>(i + 1, "robin"));
	}
	auto duration1 = timer1.stop_delta<std::milli>();
	std::cout << "����10000std::deque����ָ�� (ms)�� " << duration1 << endl;

	timer1.start();
	std::deque<Student> studentQue;
	for (int i = 0; i < 10000; i++)
	{
		studentQue.emplace_back(i + 1, "robin");
	}

	duration1 = timer1.stop_delta<std::milli>();
	std::cout << "����10000std::deque����(ms)�� " << duration1 << endl;

}

// ���������ܶ���Ա�
void test5()
{
	Timer timer1;
	timer1.start();

	// �ڴ�й¶
	Student * it = nullptr;
	for (int i = 0; i < 10000; i++)
	{
		it = new Student(i + 1, "robin");
	}

	auto duration1 = timer1.stop_delta<std::milli>();
	std::cout << "����10000 ��ʵ��(ms)�� " << duration1 << endl;

	timer1.start();

	std::shared_ptr<Student> ptr[10000];
	for (int i = 0; i < 10000; i++)
	{
		ptr[i] = make_shared<Student>(i + 1, "robin");
		//std::shared_ptr<Student> it = make_shared<Student>(i + 1, "robin");
	}

	auto duration2 = timer1.stop_delta<std::milli>();
	std::cout << "����10000 ����ʵ��(ms)�� " << duration2 << endl;
}

///////////////////////////////////////////////////////////////////
spin_lock splock;
std::mutex myMutex;
int num = 0;
std::atomic<int> count1(0);
void addFunc1()
{
	for (int i = 0; i < 1000000; ++i)
	{
		//std::lock_guard<spin_lock> lock(splock); // �������ﲻһ��������
		//num++;
		count1++;
	}
}

void addFunc2()
{
	for (int i = 0; i < 1000000; ++i)
	{
		std::lock_guard<std::mutex> lock(myMutex); // �������ﲻһ��������
		num++;

	}
}

void testThreads()
{
	Timer timer1;
	timer1.start();
	thread t1(addFunc1);
	thread t2(addFunc1);
	thread t3(addFunc1);
	t1.join();
	t2.join();
	t3.join();
	auto duration2 = timer1.stop_delta<std::milli>();
	std::cout << "��ʱ(ms)�� " << duration2 << endl;
	std::cout << count1 << endl;
}
