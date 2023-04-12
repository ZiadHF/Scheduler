#pragma once
#include "Node.h"
template <typename T>
class LinkedList
{
	Node<T>* head,tail;
public:
	LinkedList() : head(nullptr),tail(nullptr){}
	bool IsEmpty() {
		if (head)
			return false;
		return true;
	}
	bool RemoveHead(Process& x) {
		if (IsEmpty()) {
			return false;
		}
		x = head->getItem();
		head = head->getNext();
		return true;
	}
	bool FindByID(int pID, Process* x) {
		if (IsEmpty()) {
			return false;
		}
		Node<Process>* curr = head;
		while (curr->getNext() != null) {
			if (curr->getItem().getID() == pID) {
				x = &curr->getItem();
				return true;
			}
			curr = curr->getNext();
		}
		if (curr->getItem().getID == pId) {
			 x = &curr->getItem();
			return true;
		}
		return false;
	}
	bool Insert(T item) {
		Node<T>* ptr = new Node(item);
		if (!ptr)
			return false;
		if (IsEmpty) {
			head = ptr;
			tail = ptr;
			return true;
		}
		tail->setNext(ptr);
		tail = ptr;
	}
	bool Remove(T& item) {
		if (IsEmpty)
			return false;
		item = head->getItem();
		Node<T>* ptr = head;
		head = head->getNext();
		return true;
	}
	bool RemoveByID(int pID,Process& pro) {
		Node<Process>* ptr = head;
		Node<Process>* prevNode = head;
		while (ptr) {
			Process p = ptr->getItem();
			if (p.getID() == pID) {
				pro = p;
				prevNode->setNext(ptr->getNext());
				return true;
			}
			prevNode = ptr;
			ptr = ptr->getNext();
		}
		return false;
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

