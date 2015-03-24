#ifndef MYCONTAINER
#define MYCONTAINER
#include"myQueue.h"
#include"myStack.h"

using namespace std;
template<class T>
class MyContainer{
public:
	MyContainer(){}



	myQueue<T> queue;
	myStack<T> stack;
};
#endif