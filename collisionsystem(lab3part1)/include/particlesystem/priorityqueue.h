#pragma once

#include <iostream>
#include <vector>
#include <cassert>

//#define TEST_PRIORITY_QUEUE

/**
 * A heap based priority queue where the root is the smallest element -- min heap
 */
template <class Comparable>
class PriorityQueue {
public:
    /**
     * Constructor to create a queue with the given capacity
     */
    explicit PriorityQueue(int initCapacity = 100);

    /**
     * Constructor to initialize a priority queue based on a given vector V
     */
    explicit PriorityQueue(const std::vector<Comparable>& V);

    // Disable copying
    PriorityQueue(const PriorityQueue&) = delete;
    PriorityQueue& operator=(const PriorityQueue&) = delete;

    /**
     * Make the queue empty
     */
    void makeEmpty();

    /**
     * Check is the queue is empty
     * Return true if the queue is empty, false otherwise
     */
    bool isEmpty() const;

    /**
     * Get the size of the queue, i.e. number of elements in the queue
     */
    size_t size() const;

    /**
     * Get the smallest element in the queue
     */
    Comparable findMin();

    /**
     * Remove and return the smallest element in the queue
     */
    Comparable deleteMin();

    /**
     * Add a new element x to the queue
     */
    void insert(const Comparable& x);

private:
    std::vector<Comparable> pq;

    // Auxiliary member functions

    /**
     * Restore the heap property
     */
    void heapify();

    /**
     * Test whether pq is a min heap
     */
    bool isMinHeap() const;

    void perculateDown(int i);
    void perculateUp(int i);
};

/* *********************** Member functions implementation *********************** */

/**
 * Constructor to create a queue with the given capacity
 */
template <class Comparable>
PriorityQueue<Comparable>::PriorityQueue(int initCapacity) {
    /*
     * ADD CODE HERE
     */ 
    
    pq.reserve(initCapacity + 1);
    pq.push_back(Comparable());
    assert(isEmpty());  // do not remove this line
}

/**
 * Constructor to initialize a priority queue based on a given vector V
 */
template <class Comparable>
PriorityQueue<Comparable>::PriorityQueue(const std::vector<Comparable>& V) : pq{V} {
    // Implementation is provided for you
    heapify();
#ifdef TEST_PRIORITY_QUEUE
    assert(isMinHeap());
#endif
}

/**
 * Make the queue empty
 */
template <class Comparable>
void PriorityQueue<Comparable>::makeEmpty() {
    /*
     * ADD CODE HERE
     */
    for (int i = size(); i > 0; --i) {
        pq.popback();
    }
}

/**
 * Check is the queue is empty
 * Return true if the queue is empty, false otherwise
 */
template <class Comparable>
bool PriorityQueue<Comparable>::isEmpty() const {
    /*
     * ADD CODE HERE
     */
    if (std::ssize(pq) == 1) return true;

    return false;  // replace this line by the correct return value
}

/**
 * Get the size of the queue, i.e. number of elements in the queue
 */
template <class Comparable>
size_t PriorityQueue<Comparable>::size() const {
    /*
     * ADD CODE HERE
     */
    return std::ssize(pq) - 1;  // replace this line by the correct return value
}

/**
 * Get the smallest element in the queue
 */
template <class Comparable>
Comparable PriorityQueue<Comparable>::findMin() {
    assert(isEmpty() == false);  // do not remove this line
    /*
     * ADD CODE HERE
     */
    return pq[1];  // replace this line by the correct return value
}

/**
 * Remove and return the smallest element in the queue
 */
template <class Comparable>
Comparable PriorityQueue<Comparable>::deleteMin() {
    assert(!isEmpty());  // do not remove this line
    /*
     * ADD CODE HERE
     */
    Comparable min = pq[1];
    int n = size();
    std::swap(pq[1], pq[n]);
    pq.pop_back();
    if (size() > 0) {  // Only call perculateDown if there are elements left
        perculateDown(1);
    }
    return min;

    // Do not remove this code block
#ifdef TEST_PRIORITY_QUEUE
    assert(isMinHeap());
#endif

    return Comparable{};  // replace this line by the correct return value
}

/**
 * Add a new element x to the queue
 */
template <class Comparable>
void PriorityQueue<Comparable>::insert(const Comparable& x) {
    /*
     * ADD CODE HERE
     */
    pq.push_back(x);
    int i = size();
    perculateUp(i);

    // Do not remove this code block
#ifdef TEST_PRIORITY_QUEUE
    assert(isMinHeap());
#endif
}

/* ******************* Private member functions ********************* */

/**
 * Restore the heap property
 */
template <class Comparable>
void PriorityQueue<Comparable>::heapify() {
    assert(pq.size() > 1);  // do not remove this line

    /*
     * ADD CODE HERE
     */
    int n = size();  // slot zero not used
    for (int i = n / 2; i >= 1; --i) percolateDown(i);
}

/**
 * Test whether pq is a min heap
 */
template <class Comparable>
bool PriorityQueue<Comparable>::isMinHeap() const {
    /*
     * ADD CODE HERE
    */
    for (int i = size(); i > 1 ; --i) {
        if (pq[i] < pq[i/2]) {
            return false;
        }
    }
    return true;  // replace this line by the correct return value
}

template <class Comparable>
void PriorityQueue<Comparable>::perculateDown(int i) {
    Comparable temp = pq[i];
    int child = 2 * i;          // left child
    int n = size();             // slot zero not used
    while (child <= n)          // is node i a leaf?
    {
        if (child < n)  // does i have a right child node?
        {
            if (pq[child + 1] < pq[child]) child++;  // smallest child
        }
        // percolate down
        if (pq[child] < temp) {
            pq[i] = pq[child];
            i = child;
            child = 2 * i;  // left child
        } else
            break;
    }
    pq[i] = temp;
}

template <class Comparable>
void PriorityQueue<Comparable>::perculateUp(int i) {
    Comparable temp = pq[i];
    for (int j = size()/2; j >= 1; j / 2) {
        if (pq[j] > temp) {
            std::swap(pq[i], pq[j]);
            i = i / 2;
            j = j / 2;
        }
        else
            break;
    }
}