#ifndef MYQUEUE_H
#define MYQUEUE_H
#include <cstddef>
#include <iostream>
using namespace std;
template<class T>
class myQueue
{
    public:
        myQueue();
        bool empty();
        void push(T data);
        void pop();
		int size();
		T front();

    private:
        struct Node{
            T data;
            Node *next;
        };

        Node *frontAddress;
        Node *backAddress;
        int length;

};

template<class T>
myQueue<T>::myQueue(){
	frontAddress=NULL;
	backAddress=NULL;
	length=0;
}

template<class T>
bool myQueue<T>::empty(){
    if(length==0)
		return true;
	return false;
}

template<class T>
void myQueue<T>::push(T x){
    Node *newNode = new Node;
    newNode->data = x;
    newNode->next = NULL;
    if(empty()){
        frontAddress = newNode;
    }
    else{
		backAddress->next = newNode;
    }
    backAddress = newNode;
    length++;
}

template<class T>
void myQueue<T>::pop(){
    if(!empty()){
		Node *temp = frontAddress;
		if(frontAddress == backAddress){
			frontAddress = NULL;
			backAddress = NULL;
		}
		else{
			frontAddress = frontAddress->next;
		}
		delete temp;
		length--;
	}
}

template<class T>
int myQueue<T>::size(){
	return length;
}


template<class T>
T myQueue<T>::front(){
	return frontAddress->data;
}

#endif // MYQUEUE_H 