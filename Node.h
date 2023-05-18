#pragma once
#include "Process.h"
#include"Structs.h"
template <typename T>
class Node {
	T p;
	Node* next;
public:
	Node(T p) : p(p), next(nullptr) {}
	void setNext(Node* next) { this->next = next; }
	void setItem(T p) {	this->p = p; }
	T getItem() { return p; }
	Node* getNext() { return next; }
};