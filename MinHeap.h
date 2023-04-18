#include <iostream>
#include "Process.h"
using namespace std;
#pragma once

class MinHeap {
    Process** Heap;
    int CAPACITY;
    int size;
public:
    MinHeap(int c) : size(0), CAPACITY(c) {
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
        while (i != 0 && (Heap[Parent(i)]->getCT()) > (Heap[i]->getCT())) {
            swap(Heap[i], Heap[Parent(i)]);
            i = Parent(i);
        }
        return true;
    }

    void Heapify(int i) {
        int l = ChildL(i);
        int r = ChildR(i);
        int smallest = i;
        if (l < size && (Heap[l]->getCT()) < (Heap[i]->getCT()))
            smallest = l;
        if (r < size && (Heap[r]->getCT()) < (Heap[smallest]->getCT()))
            smallest = r;
        if (smallest != i) {
            swap(Heap[i], Heap[smallest]);
            Heapify(smallest);
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
        cout << endl;
    }
    Process* PeekMin() { return Heap[0]; }

    ~MinHeap() { delete[] Heap; }
};