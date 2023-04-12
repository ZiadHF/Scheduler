#include <iostream>
using namespace std;
#pragma once

class MinHeap {
    int* Heap;
    int CAPACITY;
    int size;
public:
    MinHeap(int c) : size(0), CAPACITY(c) {
        Heap = new int[CAPACITY];
    }

    int Parent(int i) { return ((i - 1) / 2); }

    int ChildL(int i) { return (2 * i + 1); }

    int ChildR(int i) { return (2 * i + 2); }

    bool Insert(int key) {
        if (size == CAPACITY) {
            return false;
        }
        int i = size;
        Heap[size++] = key;
        while (i != 0 && Heap[Parent(i)] > Heap[i]) {
            swap(Heap[i], Heap[Parent(i)]);
            i = Parent(i);
        }
        return true;
    }

    void Heapify(int i) {
        int l = ChildL(i);
        int r = ChildR(i);
        int smallest = i;
        if (l < size && Heap[l] < Heap[i])
            smallest = l;
        if (r < size && Heap[r] < Heap[smallest])
            smallest = r;
        if (smallest != i) {
            swap(Heap[i], Heap[smallest]);
            Heapify(smallest);
        }
    }

    bool IsEmpty() { return size == 0; }

    int getMin() {
        if (IsEmpty())
            return -1;
        if (size == 1) {
            size--;
            return Heap[0];
        }
        int root = Heap[0];
        Heap[0] = Heap[size - 1];
        size--;
        Heapify(0);
        return root;
    }

    ~MinHeap() { delete[] Heap; }
};