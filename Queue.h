#pragma once
#include "Node.h"
template <typename T>
class Queue
{
	Node<T>* head;
	Node<T>* tail;
public:
	Queue() : head(nullptr), tail(nullptr) {}
	bool IsEmpty() {
		if (head)
			return false;
		return true;
	}
	bool Enqueue(T item) {
		Node<T>* ptr = new Node<T>(item);
		if (!ptr)
			return false;
		if (IsEmpty()) {
			head = ptr;
			tail = ptr;
			return true;
		}
		tail->setNext(ptr);
		tail = ptr;
	}
	bool Dequeue(T& item) {
		if (IsEmpty())
			return false;
		item = head->getItem();
		Node<T>* ptr = head;
		head = head->getNext();
		return true;
	}
	~Queue(){
		Node<T>* ptr = head;
		while (ptr){
			Node<T>* tmp = ptr;
			ptr = ptr->getNext();
			delete tmp;
		}
	}
	T Peek();
};
