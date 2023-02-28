#ifndef _QUEUQE_H_
#define _QUEUQE_H_

#include <iostream>
#include <memory>
#include <iterator>

template<class T> class QueueIterator;
template<class T> class ConstQueueIterator;


/**************************
            Queue
***************************/
template<class T>
class Queue{
private:
    struct Node;
    using queueNode = std::unique_ptr<Node>;

    struct Node{
        T data;
        queueNode next = nullptr;

        Node(T _data) : data(_data) { }
    };

    queueNode first;

public:
    Queue() { }
    explicit Queue(const std::initializer_list<T>&);
    explicit Queue(const Queue&);

    void push(const T &value);
    void pop();
    T top() const;
    void print() const;
    size_t size() const;

    T &operator[](size_t index);
    const T &operator[](size_t index) const;

    QueueIterator<T> begin();
    QueueIterator<T> end();
    ConstQueueIterator<T> cbegin() const;
    ConstQueueIterator<T> cend() const;
};


/**************************
        QueueIterator
***************************/
template<class T>
class QueueIterator{
   
    //friend class Queue<T>;

    using iterator_category = std::forward_iterator_tag;
    using pointer = T*;
    using reference = T&;
    using value_type = T;

private:
    //T *m_ptr;
    Queue<T> &queue;
    int index;
 
public:

    explicit QueueIterator(Queue<T> &q, int i = 0) :
        queue(q), index(i) {  }


    reference operator*(){
        return queue[index];
    }
    
    pointer operator->(){
        return &queue[index];
    }

    reference operator*() const{
        return queue[index];
    }

    pointer operator->() const{
        return &queue[index];
    }

    QueueIterator &operator++(); //++a
    QueueIterator operator++(T); //a++
    QueueIterator &operator--();
    QueueIterator operator--(T);

    bool operator==(const QueueIterator &) const;
    bool operator!=(const QueueIterator &) const;
};

/**************************
      ConstQueueIterator
***************************/

template<class T>
class ConstQueueIterator{
   
    //friend class Queue<T>;

    using iterator_category = std::forward_iterator_tag;
    using pointer = T*;
    using reference = T&;
    using value_type = T;

private:
    //T *m_ptr;
    const Queue<T> &queue;
    int index;
 
public:

    explicit ConstQueueIterator(const Queue<T> &q, int i = 0) :
        queue(q), index(i) { }

    const T &operator*(){
        return queue[index];
    }

    const pointer operator->(){
        return &queue[index];
    }

    ConstQueueIterator &operator++(); //++a
    ConstQueueIterator operator++(T); //a++
    ConstQueueIterator &operator--();
    ConstQueueIterator operator--(T);

    bool operator==(const ConstQueueIterator &) const;
    bool operator!=(const ConstQueueIterator &) const;
};

/*****************
    Queue
******************/

template<class T>
Queue<T>::Queue(const std::initializer_list<T> &il_list){
    for(auto & val : il_list)
        push(val);
}

template<class T>
Queue<T>::Queue(const Queue &q){
    Node *tmp = q->first.get();
    while(tmp){
        this->push(tmp->data);
        tmp = tmp->next.get();
    }
}

template<class T>
void Queue<T>::push(const T &value){
    if(!first) 
        first = std::make_unique<Node>(value);
    else{
        Node *tmp = first.get();
        while(tmp->next)
            tmp = tmp->next.get();
        
        tmp->next = std::make_unique<Node>(value);
    }
}

template<class T>
void Queue<T>::pop(){
    if(!first) throw std::logic_error("Queue is empty!");

    //T tmp = first->data;

    first = std::move(first->next);
    //return tmp;
}


template<class T>
T Queue<T>::top() const{
    return first->data;
}

// with iterator
template<class T>
void Queue<T>::print() const{
    for(ConstQueueIterator<T> a = cbegin(); a != cend(); a++)
        std::cout << (*a) << " ";
    
    std::cout << std::endl;
}

template<class T>
size_t Queue<T>::size() const{
    if(!first) return 0;
    size_t s = 0;
    Node *tmp = first.get();
    while(tmp){
        ++s;
        tmp = tmp->next.get();
    }

    return s;
}


template<class T>
T &Queue<T>::operator[](size_t index){
    if(!first)
        throw std::logic_error("Queue is empty!");
    
    if(index >= size())
        throw std::logic_error("Out of bounds!");
    
    if(index == 0) return first->data;

    Node *tmp = first->next.get();
    size_t i = 1;

    while(i != index){
        tmp = tmp->next.get();
        ++i;
    }

    return tmp->data;
}


template<class T>
const T &Queue<T>::operator[](size_t index) const{
    if(!first)
        throw std::logic_error("Queue is empty!");
    
    if(index >= size())
        throw std::logic_error("Out of bounds!");
    
    if(index == 0) return first->data;

    Node *tmp = first->next.get();
    size_t i = 1;

    while(i != index){
        tmp = tmp->next.get();
        ++i;
    }

    return tmp->data;
}


template<class T>
QueueIterator<T> Queue<T>::begin(){
    return QueueIterator(*this, 0);
}

template<class T>
QueueIterator<T> Queue<T>::end(){
    return QueueIterator<T>(*this, size());
}


template<class T>
ConstQueueIterator<T> Queue<T>::cbegin() const{
    return ConstQueueIterator(*this, 0);
}

template<class T>
ConstQueueIterator<T> Queue<T>::cend() const{
    return ConstQueueIterator<T>(*this, size());
}

/*************************
        QueueIterator
**************************/

template<class T>
QueueIterator<T> &QueueIterator<T>::operator++(){
    ++index;    
    return *this;
}

template<class T>
QueueIterator<T> QueueIterator<T>::operator++(T){
    QueueIterator tmp = *this;
    ++index;
    return tmp;
}

template<class T>
QueueIterator<T> &QueueIterator<T>::operator--(){
    --index;
    return *this;
}

template<class T>
QueueIterator<T> QueueIterator<T>::operator--(T){   
    QueueIterator tmp = *this;
    --index;
    return tmp;
}

template<class T>
bool QueueIterator<T>::operator==(const QueueIterator &iter) const {
    if(iter.index != index) return false;
    if(&queue != &(iter.queue)) return false;

    return true;
}

template<class T>
bool QueueIterator<T>::operator!=(const QueueIterator &iter) const {
    if(iter.index != index) return true;
    if(&queue != &(iter.queue)) return true;

    return false;
}


/*************************
    ConstQueueIterator
**************************/

template<class T>
ConstQueueIterator<T> &ConstQueueIterator<T>::operator++(){
    ++index;    
    return *this;
}

template<class T>
ConstQueueIterator<T> ConstQueueIterator<T>::operator++(T){
    ConstQueueIterator tmp = *this;
    ++index;
    return tmp;
}

template<class T>
ConstQueueIterator<T> &ConstQueueIterator<T>::operator--(){
    --index;
    return *this;
}

template<class T>
ConstQueueIterator<T> ConstQueueIterator<T>::operator--(T){   
    ConstQueueIterator tmp = *this;
    --index;
    return tmp;
}

template<class T>
bool ConstQueueIterator<T>::operator==(const ConstQueueIterator &iter) const {
    if(iter.index != index) return false;
    if(&queue != &(iter.queue)) return false;

    return true;
}

template<class T>
bool ConstQueueIterator<T>::operator!=(const ConstQueueIterator &iter) const {
    if(iter.index != index) return true;
    if(&queue != &(iter.queue)) return true;

    return false;
}

#endif