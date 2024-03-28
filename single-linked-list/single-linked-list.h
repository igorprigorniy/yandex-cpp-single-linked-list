#pragma once
#include <iostream>
#include <iterator>
#include <cassert>

// Класс односвязного списка
template <typename Type>
class SingleLinkedList {
private:
    // Структура узла списка
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next) : value(val), next_node(next) { }

        Type value{};
        Node* next_node = nullptr;
    };

    // Класс итератора/константного итератора (ValueType=Type/const Type) списка
    template <typename ValueType>
    class BasicIterator {
        // Предоставим классу списка доступ к приватным полям и методам
        friend class SingleLinkedList;

    public:
        // Описание типа итератора
        using iterator_category = std::forward_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType*;
        using reference = ValueType&;

        // Конструктор по умолчанию
        BasicIterator() = default;

        // Конструктор копирования
        BasicIterator(const BasicIterator<Type>& other) noexcept : node_(other.node_) { }

        // Оператор присваивания
        BasicIterator& operator = (const BasicIterator& rhs) = default;

        // Операторы сравнения неконстантных итераторов (ValueType = Type)
        [[nodiscard]] bool operator == (const BasicIterator<Type>& rhs) const noexcept { return node_ == rhs.node_; }
        [[nodiscard]] bool operator != (const BasicIterator<Type>& rhs) const noexcept { return !(*this == rhs); }

        // Операторы сравнения константных итераторов (ValueType = const Type)
        [[nodiscard]] bool operator == (const BasicIterator<const Type>& rhs) const noexcept { return node_ == rhs.node_; }
        [[nodiscard]] bool operator != (const BasicIterator<const Type>& rhs) const noexcept { return !(*this == rhs); }

        // Преинкремент
        BasicIterator& operator ++ () noexcept {
            assert(node_);
            node_ = node_->next_node;
            return *this;
        }

        // Постинкремент
        BasicIterator operator ++ (int) noexcept {
            BasicIterator tmp(*this);
            ++(*this);
            return tmp;
        }

        // Оператор разыменовывания
        [[nodiscard]] reference operator * () const noexcept {
            assert(node_);
            return node_->value;
        }

        // Оператор доступа к членам
        [[nodiscard]] pointer operator -> () const noexcept {
            assert(node_);
            return &(node_->value);
        }

    private:
        // Указатель на узел
        Node* node_ = nullptr;

        // Конструктор, создающий итератор из указателя на узел (используется в классе списка)
        explicit BasicIterator(Node* node) : node_(node) { }
    };

public:
    // Конструктор по умолчанию создаёт пустой список
    SingleLinkedList() noexcept = default;

    // Конструктор, создающий список из элементов std::initializer_list
    SingleLinkedList(std::initializer_list<Type> values) {
        CopyAndSwapFromIteratorRage(values.begin(), values.end());
    }

    // Конструктор копирования
    SingleLinkedList(const SingleLinkedList& other) {
        CopyAndSwapFromIteratorRage(other.begin(), other.end());
    }

    // Деструктор
    ~SingleLinkedList() noexcept { Clear(); }

    // Оператор присваивания
    SingleLinkedList& operator = (const SingleLinkedList& rhs) {
        if (this != &rhs) {
            SingleLinkedList<Type> rhs_copy(rhs);
            this->swap(rhs_copy);
        }
        return *this;
    }

    // Псевдонимы для типов, ссылок, константных ссылок, итераторов и константных итераторов
    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;

    using Iterator      = BasicIterator<Type>;
    using ConstIterator = BasicIterator<const Type>;

    // Итераторы, указывающие на начало, конец и фиктивный узел списка
    [[nodiscard]] Iterator begin()        noexcept { return Iterator(head_.next_node); }
    [[nodiscard]] Iterator end()          noexcept { return Iterator(nullptr); }
    [[nodiscard]] Iterator before_begin() noexcept { return Iterator(&head_); }

    // Константные итераторы, указывающие на начало, конец и фиктивный узел списка
    [[nodiscard]] ConstIterator begin()        const noexcept { return cbegin(); }
    [[nodiscard]] ConstIterator end()          const noexcept { return cend(); }
    [[nodiscard]] ConstIterator before_begin() const noexcept { return cbefore_begin(); }

    [[nodiscard]] ConstIterator cbegin()        const noexcept { return Iterator(const_cast<Node*>(head_.next_node)); }
    [[nodiscard]] ConstIterator cend()          const noexcept { return Iterator(nullptr); }
    [[nodiscard]] ConstIterator cbefore_begin() const noexcept { return Iterator(const_cast<Node*>(&head_)); }

    // Функция обмена с другим списком
    void swap(SingleLinkedList& other) noexcept {
        std::swap(head_.next_node, other.head_.next_node);
        std::swap(size_, other.size_);
    }

    // Функция получения размера
    [[nodiscard]] size_t GetSize() const noexcept { return size_; }

    // Функция проверки на пустоту
    [[nodiscard]] bool IsEmpty() const noexcept { return size_ == 0u; }

    // Функция добавления в начало списка
    void PushFront(const Type& value) {
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }

    // Функция удаления из начала списка
    void PopFront() noexcept {
        assert(!IsEmpty());

        Node* tmp = head_.next_node->next_node;
        delete head_.next_node;
        head_.next_node = tmp;
        --size_;
    }

    // Функция добавления в конец списка
    // Это не просили делать, но пусть будет (в остальных методах не используется)
    void PushBack(const Type& value) {
        Node* back = &head_;
        while (back->next_node) { back = back->next_node;}

        back->next_node = new Node(value, nullptr);
        ++size_;
    }

    // Функция удаления из конца списка
    // Это не просили делать, но пусть будет (в остальных методах не используется)
    void PopBack() noexcept  {
        assert(!IsEmpty());

        Node* pre_back = &head_;
        while (pre_back->next_node->next_node) { pre_back = pre_back->next_node; }

        delete pre_back->next_node;
        pre_back->next_node = nullptr;
        --size_;
    }

    // Функция добавления после элемента, на который указывает итератор
    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        assert(pos.node_);

        pos.node_->next_node = new Node(value, pos.node_->next_node);
        ++size_;
        return Iterator(pos.node_->next_node);
    }

    // Функция удаления после элемента, на который указывает итератор
    Iterator EraseAfter(ConstIterator pos) noexcept {
        assert(pos.node_);
        assert(pos.node_->next_node);

        Node* tmp = pos.node_->next_node->next_node;
        delete pos.node_->next_node;
        pos.node_->next_node = tmp;
        --size_;

        return Iterator(tmp);
    }

    // Очистка списка
    void Clear() noexcept {
        while (head_.next_node) {
            Node* tmp = head_.next_node->next_node;
            delete head_.next_node;
            head_.next_node = tmp;
        }

        size_ = 0u;
    }

private:
    Node head_;        // Фиктивный узел
    size_t size_ = 0u; // Размер списка

    // Функция для реализации идеомы copy-and-swap в конструкторе: создаёт список, инициализированный
    // элементами в интервале [begin; end) и меняет его местами с текущим
    template <typename ContainerIterator>
    void CopyAndSwapFromIteratorRage(const ContainerIterator begin, const ContainerIterator end) {
        SingleLinkedList<Type> tmp;

        Node* back = &tmp.head_;

        for (ContainerIterator it = begin; it != end; ++it) {
            back->next_node = new Node(*it, nullptr);
            back = back->next_node;
            ++tmp.size_;
        }

        this->swap(tmp);
    }   
};

// Функция обмена формата swap(lhs, rhs)
template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

// Оператор сравнения списков "=="
template <typename Type>
bool operator == (const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
}

// Оператор сравнения списков "<"
template <typename Type>
bool operator < (const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
}

// Производные от "==" и "<" операторы сравнения списков
template <typename Type>
bool operator != (const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) { return !(lhs == rhs); }

template <typename Type>
bool operator >  (const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) { return rhs < lhs; }

template <typename Type>
bool operator <= (const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) { return !(rhs < lhs); }

template <typename Type>
bool operator >= (const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) { return !(lhs < rhs); }

// Оператор "<<" для вывода содержимого списка в поток
template <typename Type>
std::ostream& operator << (std::ostream& os, const SingleLinkedList<Type>& single_linked_list) {
    using namespace std::literals;
    os << "{"s;
    bool first = true;
    for (const Type& element : single_linked_list) {
        if (!first) os << ", "s;
        else        first = false;
        os << element;
    }
    os << "}"s;
    return os;
}