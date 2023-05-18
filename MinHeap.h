#include <iostream>
#include "Process.h"
using namespace std;
#pragma once

class MinHeap {
    Process** Heap;
    int CAPACITY;
    int size;
    bool SJF;
public:
    MinHeap(int c,bool ch) : size(0), CAPACITY(c) , SJF(ch){
        Heap = new Process*[CAPACITY];
    }

    int getSize() { return size; }

    int Parent(int i) { return ((i - 1) / 2); }

    int ChildL(int i) { return (2 * i + 1); }

    int ChildR(int i) { return (2 * i + 2); }

    bool Insert(Process* p) {
        if (size == CAPACITY) {
            return false;
        }
        int i = size;
        Heap[size++] = p;
        if (SJF)
            while (i != 0 && (Heap[Parent(i)]->getWorkingTime()) > (Heap[i]->getWorkingTime())) {
            swap(Heap[i], Heap[Parent(i)]);
            i = Parent(i);
            }
        else
            while (i != 0 && (Heap[Parent(i)]->getDL()) > (Heap[i]->getDL())) {
                swap(Heap[i], Heap[Parent(i)]);
                i = Parent(i);
            }
        return true;
    }

    void Heapify(int i) {
        int l = ChildL(i);
        int r = ChildR(i);
        int smallest = i;
        if (SJF) {
            if (l < size && (Heap[l]->getWorkingTime()) < (Heap[i]->getWorkingTime()))
                smallest = l;
            if (r < size && (Heap[r]->getWorkingTime()) < (Heap[smallest]->getWorkingTime()))
                smallest = r;
            if (smallest != i) {
                swap(Heap[i], Heap[smallest]);
                Heapify(smallest);
            }
        }
        else {
            if (l < size && (Heap[l]->getDL()) < (Heap[i]->getDL()))
                smallest = l;
            if (r < size && (Heap[r]->getDL()) < (Heap[smallest]->getDL()))
                smallest = r;
            if (smallest != i) {
                swap(Heap[i], Heap[smallest]);
                Heapify(smallest);
            }
        }
    }

    bool IsEmpty() { return size == 0; }

    Process* getMin() {
        if (IsEmpty())
            return NULL;
        if (size == 1) {
            size--;
            return Heap[0];
        }
        Process* root = Heap[0];
        Heap[0] = Heap[size - 1];
        size--;
        Heapify(0);
        return root;
    }

    void Print() {
        for (int i = 0; i < size; i++) {
            cout << Heap[i] << " ";
        }
    }

    Process* PeekMin() { return Heap[0]; }

    ~MinHeap() { delete[] Heap; }
};
