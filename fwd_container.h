#ifndef FWD_CONTAINER_H
#define FWD_CONTAINER_H

#include <cstddef>
#include <iterator>
#include <iostream>

template <typename T>
class fwd_container {
public:
    class iterator_base;
    class const_iterator_base;
    class iterator;
    class const_iterator;

    //базовый итератор
    class iterator_base {
    public:
        using iterator_category = std::forward_iterator_tag;    //идти только вперед
        using value_type = T;                                   //элементы с которыми работает итр имеют тип T
        using difference_type = std::ptrdiff_t;                 //расстояение между двумя итераторами
        using pointer = T*;                                     //указатель
        using reference = T&;                                   //ссылка

        virtual ~iterator_base() = default;                     //удаляем обьект через родител
        virtual reference operator*() = 0;                      //берем текущий элемент итератора
        virtual pointer operator->() = 0;                       //берем текущий элемент итератора и обр к полю и методу
        virtual iterator_base& operator++() = 0;

        // 4 оператора сравнения
        virtual bool operator==(const iterator_base& o) const = 0;          //true если они равны
        virtual bool operator!=(const iterator_base& o) const = 0;          //true если они не равны
        virtual bool operator==(const const_iterator_base& o) const = 0;
        virtual bool operator!=(const const_iterator_base& o) const = 0;

    protected:
        virtual iterator_base* clone() const = 0;                           //Делает копию самого итератора
        virtual const_iterator_base* make_const() const = 0;                //делает конст версию иетратора
        friend class iterator;
        friend class const_iterator;
    };

    // базовый константный итератор
    class const_iterator_base {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;

        virtual ~const_iterator_base() = default;
        virtual reference operator*() const = 0;
        virtual pointer operator->() const = 0;
        virtual const_iterator_base& operator++() = 0;

        virtual bool operator==(const const_iterator_base& o) const = 0;
        virtual bool operator!=(const const_iterator_base& o) const = 0;
        virtual bool operator==(const iterator_base& o) const = 0;
        virtual bool operator!=(const iterator_base& o) const = 0;

    protected:
        virtual const_iterator_base* clone() const = 0;
        friend class iterator;
        friend class const_iterator;
    };

    // обертка iterator
    class iterator {
        iterator_base* ptr;     //указатель на иетратор
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        iterator();                                     //создает пустой итератор, который не указывает
        explicit iterator(iterator_base* p);            //присваивание p в ptr
        ~iterator();                                    //деструктор
        iterator(const iterator& o);                    //копирующий конструктор
        iterator(iterator&& o) noexcept;                //перемещающий конструктор
        iterator& operator=(const iterator& o);         //копирующее присваивание
        iterator& operator=(iterator&& o) noexcept;     //Перемещающее присваивание

        reference operator*();
        pointer operator->();
        iterator& operator++();
        iterator operator++(int);

        bool operator==(const iterator& o) const;
        bool operator!=(const iterator& o) const;
        bool operator==(const const_iterator& o) const;
        bool operator!=(const const_iterator& o) const;

        iterator_base* get() const;
    };

    // обертка const_iterator
    class const_iterator {
        const_iterator_base* ptr;
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;

        const_iterator();
        explicit const_iterator(const_iterator_base* p);
        ~const_iterator();
        const_iterator(const const_iterator& o);
        const_iterator(const_iterator&& o) noexcept;
        const_iterator(const iterator& o);
        const_iterator& operator=(const const_iterator& o);
        const_iterator& operator=(const_iterator&& o) noexcept;
        const_iterator& operator=(const iterator& o);

        reference operator*() const;
        pointer operator->() const;
        const_iterator& operator++();
        const_iterator operator++(int);

        //операторы сравнения итераторов
        bool operator==(const const_iterator& o) const;
        bool operator!=(const const_iterator& o) const;
        bool operator==(const iterator& o) const;
        bool operator!=(const iterator& o) const;

        const_iterator_base* get() const;
    };

    // методы контейнера
    virtual ~fwd_container() = default;         //десткрутор
    virtual void push(const T& val) = 0;        //добавляет элемент по значению в контейнер
    virtual void push(T&& val) = 0;             //перемещает элемент
    virtual T pop() = 0;                        //удаляет и возвращает элемент из контейнера
    virtual T& get_front() = 0;                 //ссылка на 1 элемент контейнера
    virtual const T& get_front() const = 0;     //конст ссылка на 1 элемент контейнера
    virtual bool is_empty() const = 0;          //пустой контейнер
    bool empty() const { return is_empty(); }
    virtual std::size_t size() const = 0;       //кол-во элементов в контейнере
    virtual iterator begin() = 0;               //итератор на первый элемент контейнера
    virtual iterator end() = 0;
    virtual const_iterator begin() const = 0;    //конст итератор на первый элемент
    virtual const_iterator end() const = 0;      //конст итератор за последним элементом
    virtual const_iterator cbegin() const = 0;   //конст итератор на первый элемент (стандарт cbegin)
    virtual const_iterator cend() const = 0;     //конст итератор за последним элементом (стандарт cend)

    //очищает контейнер и копирует элементы из o
    virtual fwd_container& operator=(const fwd_container& o);
};

//ввод
template <typename T>
std::istream& operator>>(std::istream& is, fwd_container<T>& c);

//вывод
template <typename T>
std::ostream& operator<<(std::ostream& os, const fwd_container<T>& c);

#include "fwd_container_impl.h"

#endif
