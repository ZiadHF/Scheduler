#pragma once
#include "Node.h"
template <typename T>
class LinkedList
{
	int count;
	Node<T>* head;
	Node<T>* tail;
public:
	LinkedList() : head(nullptr),tail(nullptr),count(0){ }

	bool IsEmpty() {
		if (head)
			return false;
		return true;
	}

	bool RemoveHead(Process** x) {
		if (IsEmpty()) {
			return false;
		}
		*x = head->getItem();
		head = head->getNext();
		return true;
	}

	bool FindByID(int pID, Process* p) {
		if (IsEmpty()) {
			return false;
		}
		Node<Process*>* ptr = head;
		while (ptr) {
			Process* p = ptr->getItem();
			if (p->getID() == pID) {
				p = ptr->getItem();
				return true;
			}
			ptr = ptr->getNext();
		}
		return false;
	}

	int getCount() { return count; }

	bool Insert(T item) {
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

	bool Remove(T item) {
		if (IsEmpty())
			return false;
		item = head->getItem();
		Node<T>* ptr = head;
		head = head->getNext();
		return true;
	}

	bool RemoveByID(int pID,Process** pro) {
		Node<Process*>* ptr = head;
		Node<Process*>* prevNode = head;
		while (ptr) {
			Process* p = ptr->getItem();
			if (p->getID() == pID) {
				*pro = p;
				prevNode->setNext(ptr->getNext());
				return true;
			}
			prevNode = ptr;
			ptr = ptr->getNext();
		}
		return false;
	}

	T getFirst() { return head->getItem(); }

	void Print() {
		Node<T>* ptr = head;
		while (ptr) {
			cout << ptr->getItem() << " ";
			ptr = ptr->getNext();
		}
	}

	~LinkedList(){
		Node<T>* ptr = head;
		while (ptr){
			Node<T>* tmp = ptr;
			ptr = ptr->getNext();
			delete tmp;
		}
	}
};

