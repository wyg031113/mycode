#ifndef __DATA_H__
#define __DATA_H__
class Data
{
public:
	virtual void destory(){}
	virtual void ~Data(){}
};

class MyData:public Data
{
public:
	int x;
	double y;
};
#endif /* __DATA_H__ */
