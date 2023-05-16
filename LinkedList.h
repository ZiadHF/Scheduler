#pragma once
#include "Node.h"
class Process;
class FCFS;
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
		if (!head)
			return false;

		Node<Process*>* temp;
		if (head == tail) {
			*x = head->getItem();
			delete head;
			head = nullptr;
			tail = nullptr;
			count = 0;
			return true;
		}
		*x = head->getItem();
		temp = head;
		head = head->getNext();
		delete temp;
		count--;
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


	bool RemoveByID(int pID,Process** pro) {
		if (!head)
			return false;
		Process* temp;
		if (head == tail) {
			temp = head->getItem();
			if (temp->getID() == pID) {
				RemoveHead(pro);
				return true;
			}
		}
		temp = head->getItem();
		if (temp->getID()==pID) {
			RemoveHead(pro);
			return true;
		}
		Node<Process*>* ptr=head->getNext();
		Node<Process*>* prev = head;
		while (ptr) {
			temp = ptr->getItem();
			if (temp->getID() == pID) {
				if (ptr == tail) {
					tail = prev;
					*pro = temp;
					count--;
					tail->setNext(nullptr);
					delete ptr;
					return true;
				}
				prev->setNext(ptr->getNext());
				*pro = temp;
				delete ptr;
				count--;
				return true;
			}
			prev = ptr;
			ptr = ptr->getNext();
		}
		return false;
	}

	T getFirst() { return head->getItem(); }

	void Print() {
		Node<Process*>* ptr = head;
		while (ptr) {
			Process* temp = ptr->getItem();
			cout << temp << " ";
			ptr = ptr->getNext();
		}
	}
	
	bool CheckKillSignal(int* ID,int time) {
		if (head != nullptr) {
			Node<SIGKILL>* temp = head->getNext();
			Node<SIGKILL>* prev = head;
			if (prev->getItem().Kill_Time == time) {
				*ID = prev->getItem().Kill_PID;
				head = head->getNext();
				delete prev;
				return true;
			}
			while (temp) {
				if (temp->getItem().Kill_Time == time) {
					*ID = temp->getItem().Kill_PID;
					prev->setNext(temp->getNext());
					delete temp;
					temp = nullptr;
					return true;
				}
				prev = temp;
				temp = temp->getNext();
			}
			*ID = 0;
			return false;
		}
		return false;
	}

	bool getNextNONforked(Process** x) {
		Process* temp = head->getItem();
		if (!temp->getisForked()) {
			RemoveByID(temp->getID(),x);
			return true;
		}
		Node<Process*>* ptr = head->getNext();
		while (ptr) {
			Process* temp = ptr->getItem();
			while (temp->getisForked()) {
				ptr = ptr->getNext();
				if (!ptr)
					break;
				temp = ptr->getItem();
			}
			if (!temp->getisForked()) {
				RemoveByID(temp->getID(),x);
				return true;
			}
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

