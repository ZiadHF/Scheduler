#pragma once
#include "Node.h"
template <typename T>
class Queue
{
	int count;
	Node<T>* head;
	Node<T>* tail;
public:
	Queue() : head(nullptr), tail(nullptr) , count(0){}

	int getCount() { return count; }
	bool IsEmpty() {
		if (head)
			return false;
		return true;
	}

	bool Enqueue(T& item) {
		Node<T>* ptr = new Node<T>(item);
		if (!ptr)
			return false;
		if (IsEmpty()) {
			head = ptr;
			tail = ptr;
			count++;
			return true;
		}
		tail->setNext(ptr);
		tail = ptr;
		count++;
		return true;
	}

	bool Dequeue(T* item) {
		Node<T>* temp=head;
		if (IsEmpty())
			return false;
		*item = head->getItem();
		head = head->getNext();
		delete temp;
		count--;
		return true;
	}


	T& Peek() {
		if(head)
		return head->getItem();
		return nullptr;
	}

	void Print() {
		Process* temp;
		Node<Process*>* ptr = head;
		while (ptr) {
			temp = ptr->getItem();
			cout <<temp << " ";
			ptr = ptr->getNext();
		}
	}

	~Queue() {
		Node<T>* ptr = head;
		while (ptr) {
			Node<T>* tmp = ptr;
			ptr = ptr->getNext();
			delete tmp;
		}
	}
};
