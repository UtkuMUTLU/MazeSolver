#ifndef STLCONTAINER
#define STLCONTAINER
#include<queue>
#include<stack>


using namespace std;

template<class T>
class STLContainer{
public:
	STLContainer<T>(){}


	queue<T> queue;
	stack<T> stack;
};
#endif