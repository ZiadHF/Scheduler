#include "Process.h"
#pragma once
class ListADT {
public:
	virtual bool isEmpty() = 0;
	virtual int getLength() = 0;
	virtual bool Insert() = 0;
	virtual void Clear() = 0;
	virtual bool Remove(Process item) = 0;
};