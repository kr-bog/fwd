#ifndef STACK_IMPL_H
#define STACK_IMPL_H

//реализация stack_iterator

//конструктор
template <typename T>
stack<T>::stack_iterator::stack_iterator(Node* n): cur(n) {}

//возвращает данные узла
template <typename T>
typename stack<T>::iterator_base::reference
stack<T>::stack_iterator::operator*() { return cur->data; }

//доступ к полю
template <typename T>
typename stack<T>::iterator_base::pointer
stack<T>::stack_iterator::operator->() { return &cur->data; }

//шаг вперед
template <typename T>
typename stack<T>::stack_iterator&
stack<T>::stack_iterator::operator++() {
    if(cur) cur = cur->next;
    return *this;
}

//сравнение с итератором
template <typename T>
bool stack<T>::stack_iterator::operator==(const iterator_base& o) const {
    auto* p = dynamic_cast<const stack_iterator*>(&o);
    return p && cur == p->cur;
}

template <typename T>
bool stack<T>::stack_iterator::operator!=(const iterator_base& o) const {
    return !(*this == o);
}

//сравнение с константным итератором
template <typename T>
bool stack<T>::stack_iterator::operator==(const const_iterator_base& o) const {
    auto* p = dynamic_cast<const stack_const_iterator*>(&o);
    return p && cur == p->cur;
}

template <typename T>
bool stack<T>::stack_iterator::operator!=(const const_iterator_base& o) const {
    return !(*this == o);
}

//копия итератора
template <typename T>
typename stack<T>::iterator_base*
stack<T>::stack_iterator::clone() const {
    return new stack_iterator(*this);
}

//создает константную версию
template <typename T>
typename stack<T>::const_iterator_base*
stack<T>::stack_iterator::make_const() const {
    return new stack_const_iterator(cur);
}

//реализация stack_const_iterator

//конструктор
template <typename T>
stack<T>::stack_const_iterator::stack_const_iterator(const Node* n): cur(n) {}

//конструктор из обычного итератора
template <typename T>
stack<T>::stack_const_iterator::stack_const_iterator(const stack_iterator& o): cur(o.cur) {}

//возвращает константную ссылку
template <typename T>
typename stack<T>::const_iterator_base::reference
stack<T>::stack_const_iterator::operator*() const { return cur->data; }

//доступ к полю
template <typename T>
typename stack<T>::const_iterator_base::pointer
stack<T>::stack_const_iterator::operator->() const { return &cur->data; }

//шаг вперед
template <typename T>
typename stack<T>::stack_const_iterator&
stack<T>::stack_const_iterator::operator++() {
    if(cur) cur = cur->next;
    return *this;
}

//сравнение с константным итератором
template <typename T>
bool stack<T>::stack_const_iterator::operator==(const const_iterator_base& o) const {
    auto* p = dynamic_cast<const stack_const_iterator*>(&o);
    return p && cur == p->cur;
}

template <typename T>
bool stack<T>::stack_const_iterator::operator!=(const const_iterator_base& o) const {
    return !(*this == o);
}

//сравнение с обычным итератором
template <typename T>
bool stack<T>::stack_const_iterator::operator==(const iterator_base& o) const {
    auto* p = dynamic_cast<const stack_iterator*>(&o);
    return p && cur == p->cur;
}

template <typename T>
bool stack<T>::stack_const_iterator::operator!=(const iterator_base& o) const {
    return !(*this == o);
}

//копия итератора
template <typename T>
typename stack<T>::const_iterator_base*
stack<T>::stack_const_iterator::clone() const {
    return new stack_const_iterator(*this);
}

//реализация конструкторов и деструктора стека

//создает пустой стек
template <typename T>
stack<T>::stack(): top_(nullptr), sz_(0) {}

//деструктор
template <typename T>
stack<T>::~stack() { clear(); }

//копирующий конструктор
template <typename T>
stack<T>::stack(const stack& o): top_(nullptr), sz_(0) {
    copy_from(o);
}

//перемещающий конструктор
template <typename T>
stack<T>::stack(stack&& o): top_(o.top_), sz_(o.sz_) {
    o.top_ = nullptr;
    o.sz_ = 0;
}

//копирующее присваивание
template <typename T>
stack<T>& stack<T>::operator=(const stack& o) {
    if(this != &o) {
        clear();
        copy_from(o);
    }
    return *this;
}

//перемещающее присваивание
template <typename T>
stack<T>& stack<T>::operator=(stack&& o) {
    if(this != &o) {
        clear();
        top_ = o.top_;
        sz_ = o.sz_;
        o.top_ = nullptr;
        o.sz_ = 0;
    }
    return *this;
}

//присваивание через базовый класс
template <typename T>
fwd_container<T>& stack<T>::operator=(const fwd_container<T>& o) {
    return fwd_container<T>::operator=(o);
}

//реализация методов контейнера

//вставка копированием в вершину
template <typename T>
void stack<T>::push(const T& v) {
    top_ = new Node(v, top_);
    sz_++;
}

//вставка перемещением в вершину
template <typename T>
void stack<T>::push(T&& v) {
    top_ = new Node(std::move(v), top_);
    sz_++;
}

//удаление с вершины
template <typename T>
T stack<T>::pop() {
    if(is_empty()) throw std::runtime_error("stack empty");
    Node* tmp = top_;
    T val = std::move(top_->data);
    top_ = top_->next;
    delete tmp;
    sz_--;
    return val;
}

//доступ к вершине
template <typename T>
T& stack<T>::get_front() {
    if(is_empty()) throw std::runtime_error("stack empty");
    return top_->data;
}

//константный доступ к вершине
template <typename T>
const T& stack<T>::get_front() const {
    if(is_empty()) throw std::runtime_error("stack empty");
    return top_->data;
}

//пустой
template <typename T>
bool stack<T>::is_empty() const { return top_ == nullptr; }

//размер
template <typename T>
std::size_t stack<T>::size() const { return sz_; }

//реализация итераторов

//итератор на вершину
template <typename T>
typename stack<T>::iterator stack<T>::begin() {
    return iterator(new stack_iterator(top_));
}

//итератор на конец
template <typename T>
typename stack<T>::iterator stack<T>::end() {
    return iterator(new stack_iterator(nullptr));
}

//константный итератор на вершину
template <typename T>
typename stack<T>::const_iterator stack<T>::begin() const {
    return const_iterator(new stack_const_iterator(top_));
}

//константный итератор на конец
template <typename T>
typename stack<T>::const_iterator stack<T>::end() const {
    return const_iterator(new stack_const_iterator(nullptr));
}

//cbegin
template <typename T>
typename stack<T>::const_iterator stack<T>::cbegin() const {
    return const_iterator(new stack_const_iterator(top_));
}

//cend
template <typename T>
typename stack<T>::const_iterator stack<T>::cend() const {
    return const_iterator(new stack_const_iterator(nullptr));
}

//вспомогательные методы

//очистка стека
template <typename T>
void stack<T>::clear() {
    while(top_) {
        Node* t = top_;
        top_ = top_->next;
        delete t;
    }
    sz_ = 0;
}

//копирование за один проход O(n)
template <typename T>
void stack<T>::copy_from(const stack& o) {
    if(o.top_ == nullptr) return;

    //создаем первый узел
    top_ = new Node(o.top_->data);
    Node* new_cur = top_;
    Node* old_cur = o.top_->next;

    //копируем остальные узлы
    while(old_cur != nullptr) {
        new_cur->next = new Node(old_cur->data);
        new_cur = new_cur->next;
        old_cur = old_cur->next;
    }

    sz_ = o.sz_;
}

#endif
