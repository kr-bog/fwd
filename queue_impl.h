#ifndef QUEUE_IMPL_H
#define QUEUE_IMPL_H

//реализация queue_iterator

//конструктор
template <typename T>
queue<T>::queue_iterator::queue_iterator(Node* n): cur(n) {}

//возвращает данные узла
template <typename T>
typename queue<T>::iterator_base::reference
queue<T>::queue_iterator::operator*() { return cur->data; }

//доступ к полю
template <typename T>
typename queue<T>::iterator_base::pointer
queue<T>::queue_iterator::operator->() { return &cur->data; }

//шаг вперед
template <typename T>
typename queue<T>::queue_iterator&
queue<T>::queue_iterator::operator++() {
    if(cur) cur = cur->next;
    return *this;
}

//сравнение с итератором
template <typename T>
bool queue<T>::queue_iterator::operator==(const iterator_base& o) const {
    auto* p = dynamic_cast<const queue_iterator*>(&o);
    return p && cur == p->cur;
}

template <typename T>
bool queue<T>::queue_iterator::operator!=(const iterator_base& o) const {
    return !(*this == o);
}

//сравнение с константным итератором
template <typename T>
bool queue<T>::queue_iterator::operator==(const const_iterator_base& o) const {
    auto* p = dynamic_cast<const queue_const_iterator*>(&o);
    return p && cur == p->cur;
}

template <typename T>
bool queue<T>::queue_iterator::operator!=(const const_iterator_base& o) const {
    return !(*this == o);
}

//копия итератора
template <typename T>
typename queue<T>::iterator_base*
queue<T>::queue_iterator::clone() const {
    return new queue_iterator(*this);
}

//создает константную версию
template <typename T>
typename queue<T>::const_iterator_base*
queue<T>::queue_iterator::make_const() const {
    return new queue_const_iterator(cur);
}

//реализация queue_const_iterator

//конструктор
template <typename T>
queue<T>::queue_const_iterator::queue_const_iterator(const Node* n): cur(n) {}

//конструктор из обычного итератора
template <typename T>
queue<T>::queue_const_iterator::queue_const_iterator(const queue_iterator& o): cur(o.cur) {}

//возвращает константную ссылку
template <typename T>
typename queue<T>::const_iterator_base::reference
queue<T>::queue_const_iterator::operator*() const { return cur->data; }

//доступ к полю
template <typename T>
typename queue<T>::const_iterator_base::pointer
queue<T>::queue_const_iterator::operator->() const { return &cur->data; }

//шаг вперед
template <typename T>
typename queue<T>::queue_const_iterator&
queue<T>::queue_const_iterator::operator++() {
    if(cur) cur = cur->next;
    return *this;
}

//сравнение с константным итератором
template <typename T>
bool queue<T>::queue_const_iterator::operator==(const const_iterator_base& o) const {
    auto* p = dynamic_cast<const queue_const_iterator*>(&o);
    return p && cur == p->cur;
}

template <typename T>
bool queue<T>::queue_const_iterator::operator!=(const const_iterator_base& o) const {
    return !(*this == o);
}

//сравнение с обычным итератором
template <typename T>
bool queue<T>::queue_const_iterator::operator==(const iterator_base& o) const {
    auto* p = dynamic_cast<const queue_iterator*>(&o);
    return p && cur == p->cur;
}

template <typename T>
bool queue<T>::queue_const_iterator::operator!=(const iterator_base& o) const {
    return !(*this == o);
}

//копия итератора
template <typename T>
typename queue<T>::const_iterator_base*
queue<T>::queue_const_iterator::clone() const {
    return new queue_const_iterator(*this);
}

//реализация конструкторов и деструктора очереди

//создает пустую очередь
template <typename T>
queue<T>::queue(): front_(nullptr), back_(nullptr), sz_(0) {}

//деструктор
template <typename T>
queue<T>::~queue() { clear(); }

//копирующий конструктор
template <typename T>
queue<T>::queue(const queue& o): front_(nullptr), back_(nullptr), sz_(0) {
    copy_from(o);
}

//перемещающий конструктор
template <typename T>
queue<T>::queue(queue&& o): front_(o.front_), back_(o.back_), sz_(o.sz_) {
    o.front_ = nullptr;
    o.back_ = nullptr;
    o.sz_ = 0;
}

//копирующее присваивание
template <typename T>
queue<T>& queue<T>::operator=(const queue& o) {
    if(this != &o) {
        clear();
        copy_from(o);
    }
    return *this;
}

//перемещающее присваивание
template <typename T>
queue<T>& queue<T>::operator=(queue&& o) {
    if(this != &o) {
        clear();
        front_ = o.front_;
        back_ = o.back_;
        sz_ = o.sz_;
        o.front_ = nullptr;
        o.back_ = nullptr;
        o.sz_ = 0;
    }
    return *this;
}

//присваивание через базовый класс
template <typename T>
fwd_container<T>& queue<T>::operator=(const fwd_container<T>& o) {
    return fwd_container<T>::operator=(o);
}

//реализация методов контейнера

//вставка копированием в конец
template <typename T>
void queue<T>::push(const T& v) {
    Node* n = new Node(v);
    if(is_empty()) {
        front_ = back_ = n;
    } else {
        back_->next = n;
        back_ = n;
    }
    sz_++;
}

//вставка перемещением в конец
template <typename T>
void queue<T>::push(T&& v) {
    Node* n = new Node(std::move(v));
    if(is_empty()) {
        front_ = back_ = n;
    } else {
        back_->next = n;
        back_ = n;
    }
    sz_++;
}

//удаление из начала
template <typename T>
T queue<T>::pop() {
    if(is_empty()) throw std::runtime_error("очередь пуста");
    Node* tmp = front_;
    T val = std::move(front_->data);
    front_ = front_->next;
    if(front_ == nullptr) back_ = nullptr;
    delete tmp;
    sz_--;
    return val;
}

//доступ к первому элементу
template <typename T>
T& queue<T>::get_front() {
    if(is_empty()) throw std::runtime_error("queue empty");
    return front_->data;
}

//константный доступ к первому элементу
template <typename T>
const T& queue<T>::get_front() const {
    if(is_empty()) throw std::runtime_error("queue empty");
    return front_->data;
}

//пустой
template <typename T>
bool queue<T>::is_empty() const { return front_ == nullptr; }

//размер
template <typename T>
std::size_t queue<T>::size() const { return sz_; }

//реализация итераторов

//итератор на начало
template <typename T>
typename queue<T>::iterator queue<T>::begin() {
    return iterator(new queue_iterator(front_));
}

//итератор на конец
template <typename T>
typename queue<T>::iterator queue<T>::end() {
    return iterator(new queue_iterator(nullptr));
}

//константный итератор на начало
template <typename T>
typename queue<T>::const_iterator queue<T>::begin() const {
    return const_iterator(new queue_const_iterator(front_));
}

//константный итератор на конец
template <typename T>
typename queue<T>::const_iterator queue<T>::end() const {
    return const_iterator(new queue_const_iterator(nullptr));
}

//cbegin
template <typename T>
typename queue<T>::const_iterator queue<T>::cbegin() const {
    return const_iterator(new queue_const_iterator(front_));
}

//cend
template <typename T>
typename queue<T>::const_iterator queue<T>::cend() const {
    return const_iterator(new queue_const_iterator(nullptr));
}

//вспомогательные методы

//очистка очереди
template <typename T>
void queue<T>::clear() {
    while(front_) {
        Node* t = front_;
        front_ = front_->next;
        delete t;
    }
    back_ = nullptr;
    sz_ = 0;
}

//копирование за один проход O(n)
template <typename T>
void queue<T>::copy_from(const queue& o) {
    Node* cur = o.front_;
    while(cur) {
        push(cur->data);
        cur = cur->next;
    }
}

#endif
