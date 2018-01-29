/*
这里报错，需要添加宏定义
*/
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <stdio.h>
#include <string>
using namespace std;

//定义一个枚举类型，包含File,Queue,Pathway等类型
enum PersistenceType
{
	File, Queue, Pathway
};
//定义一个结构体
struct PersistenceAttribute
{
	PersistenceType type;
	char value[30];
};
//定义一个DistrWorkPackage类
class DistrWorkPackage
{
public:
	//构造方法
	DistrWorkPackage(char *type)
	{
		sprintf(_desc, "Distributed Work Package for: %s", type);
	}
	//设置文件的相关属性
	void setFile(char *f, char *v)
	{
		sprintf(_temp, "\n  File(%s): %s", f, v);
		strcat(_desc, _temp);
	}
	//设置队列的相关属性
	void setQueue(char *q, char *v)
	{
		sprintf(_temp, "\n  Queue(%s): %s", q, v);
		strcat(_desc, _temp);
	}
	//设置路径的相关属性
	void setPathway(char *p, char *v)
	{
		sprintf(_temp, "\n  Pathway(%s): %s", p, v);
		strcat(_desc, _temp);
	}
	const char *getState()
	{
		return _desc;
	}
private:
	char _desc[200];
	char _temp[80];
};
//抽象构造器
class Builder
{
public:
	virtual void configureFile(char*) = 0;
	virtual void configureQueue(char*) = 0;
	virtual void configurePathway(char*) = 0;
	DistrWorkPackage *getResult()
	{
		return _result;
	}
protected:
	//在Builder类里定义一个DistrWorkPackage对象
	DistrWorkPackage *_result;
};
//UnixBuilder类定义，继承Builder类
class UnixBuilder : public Builder
{
public:
	UnixBuilder()
	{
		_result = new DistrWorkPackage("Unix");
	}
	void configureFile(char *name)
	{
		_result->setFile("flatFile", name);
	}
	void configureQueue(char *queue)
	{
		_result->setQueue("FIFO", queue);
	}
	void configurePathway(char *type)
	{
		_result->setPathway("thread", type);
	}
};
//VmxBuilder类定义，继承Builder类
class VmsBuilder : public Builder
{
public:
	VmsBuilder()
	{
		_result = new DistrWorkPackage("Vms");
	}
	void configureFile(char *name)
	{
		_result->setFile("ISAM", name);
	}
	void configureQueue(char *queue)
	{
		_result->setQueue("priority", queue);
	}
	void configurePathway(char *type)
	{
		_result->setPathway("LWP", type);
	}
};

//定义一个解析器类
class Reader
{
public:
	void setBuilder(Builder *b)
	{
		_builder = b;
	}
	void construct(PersistenceAttribute[], int);
private:
	//在解析器类里，定义一个Builder对象，用来操作其子类
	Builder *_builder;
};

void Reader::construct(PersistenceAttribute list[], int num)
{
	for (int i = 0; i < num; i++)
		if (list[i].type == File)
			_builder->configureFile(list[i].value);
		else if (list[i].type == Queue)
			_builder->configureQueue(list[i].value);
		else if (list[i].type == Pathway)
			_builder->configurePathway(list[i].value);
}

const int NUM_ENTRIES = 6;
PersistenceAttribute input[NUM_ENTRIES] =
{
	{
		File, "state.dat"
	}
	,
	{
		File, "config.sys"
	}
	,
	{
		Queue, "compute"
	}
	,
	{
		Queue, "log"
	}
	,
	{
		Pathway, "authentication"
	}
	,
	{
		Pathway, "error processing"
	}
};

int main()
{
	
	UnixBuilder unixBuilder;
	VmsBuilder vmsBuilder;
	Reader reader;

	reader.setBuilder(&unixBuilder);
	reader.construct(input, NUM_ENTRIES);
	cout << unixBuilder.getResult()->getState() << endl;

	reader.setBuilder(&vmsBuilder);
	reader.construct(input, NUM_ENTRIES);
	cout << vmsBuilder.getResult()->getState() << endl;


	system("pause");
	return 0;
}