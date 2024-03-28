#pragma once
#include <iostream>
#include <iterator>
#include <cassert>

// ����� ������������ ������
template <typename Type>
class SingleLinkedList {
private:
    // ��������� ���� ������
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next) : value(val), next_node(next) { }

        Type value{};
        Node* next_node = nullptr;
    };

    // ����� ���������/������������ ��������� (ValueType=Type/const Type) ������
    template <typename ValueType>
    class BasicIterator {
        // ����������� ������ ������ ������ � ��������� ����� � �������
        friend class SingleLinkedList;

    public:
        // �������� ���� ���������
        using iterator_category = std::forward_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType*;
        using reference = ValueType&;

        // ����������� �� ���������
        BasicIterator() = default;

        // ����������� �����������
        BasicIterator(const BasicIterator<Type>& other) noexcept : node_(other.node_) { }

        // �������� ������������
        BasicIterator& operator = (const BasicIterator& rhs) = default;

        // ��������� ��������� ������������� ���������� (ValueType = Type)
        [[nodiscard]] bool operator == (const BasicIterator<Type>& rhs) const noexcept { return node_ == rhs.node_; }
        [[nodiscard]] bool operator != (const BasicIterator<Type>& rhs) const noexcept { return !(*this == rhs); }

        // ��������� ��������� ����������� ���������� (ValueType = const Type)
        [[nodiscard]] bool operator == (const BasicIterator<const Type>& rhs) const noexcept { return node_ == rhs.node_; }
        [[nodiscard]] bool operator != (const BasicIterator<const Type>& rhs) const noexcept { return !(*this == rhs); }

        // ������������
        BasicIterator& operator ++ () noexcept {
            assert(node_);
            node_ = node_->next_node;
            return *this;
        }

        // �������������
        BasicIterator operator ++ (int) noexcept {
            BasicIterator tmp(*this);
            ++(*this);
            return tmp;
        }

        // �������� ���������������
        [[nodiscard]] reference operator * () const noexcept {
            assert(node_);
            return node_->value;
        }

        // �������� ������� � ������
        [[nodiscard]] pointer operator -> () const noexcept {
            assert(node_);
            return &(node_->value);
        }

    private:
        // ��������� �� ����
        Node* node_ = nullptr;

        // �����������, ��������� �������� �� ��������� �� ���� (������������ � ������ ������)
        explicit BasicIterator(Node* node) : node_(node) { }
    };

public:
    // ����������� �� ��������� ������ ������ ������
    SingleLinkedList() noexcept = default;

    // �����������, ��������� ������ �� ��������� std::initializer_list
    SingleLinkedList(std::initializer_list<Type> values) {
        CopyAndSwapFromIteratorRage(values.begin(), values.end());
    }

    // ����������� �����������
    SingleLinkedList(const SingleLinkedList& other) {
        CopyAndSwapFromIteratorRage(other.begin(), other.end());
    }

    // ����������
    ~SingleLinkedList() noexcept { Clear(); }

    // �������� ������������
    SingleLinkedList& operator = (const SingleLinkedList& rhs) {
        if (this != &rhs) {
            SingleLinkedList<Type> rhs_copy(rhs);
            this->swap(rhs_copy);
        }
        return *this;
    }

    // ���������� ��� �����, ������, ����������� ������, ���������� � ����������� ����������
    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;

    using Iterator      = BasicIterator<Type>;
    using ConstIterator = BasicIterator<const Type>;

    // ���������, ����������� �� ������, ����� � ��������� ���� ������
    [[nodiscard]] Iterator begin()        noexcept { return Iterator(head_.next_node); }
    [[nodiscard]] Iterator end()          noexcept { return Iterator(nullptr); }
    [[nodiscard]] Iterator before_begin() noexcept { return Iterator(&head_); }

    // ����������� ���������, ����������� �� ������, ����� � ��������� ���� ������
    [[nodiscard]] ConstIterator begin()        const noexcept { return cbegin(); }
    [[nodiscard]] ConstIterator end()          const noexcept { return cend(); }
    [[nodiscard]] ConstIterator before_begin() const noexcept { return cbefore_begin(); }

    [[nodiscard]] ConstIterator cbegin()        const noexcept { return Iterator(const_cast<Node*>(head_.next_node)); }
    [[nodiscard]] ConstIterator cend()          const noexcept { return Iterator(nullptr); }
    [[nodiscard]] ConstIterator cbefore_begin() const noexcept { return Iterator(const_cast<Node*>(&head_)); }

    // ������� ������ � ������ �������
    void swap(SingleLinkedList& other) noexcept {
        std::swap(head_.next_node, other.head_.next_node);
        std::swap(size_, other.size_);
    }

    // ������� ��������� �������
    [[nodiscard]] size_t GetSize() const noexcept { return size_; }

    // ������� �������� �� �������
    [[nodiscard]] bool IsEmpty() const noexcept { return size_ == 0u; }

    // ������� ���������� � ������ ������
    void PushFront(const Type& value) {
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }

    // ������� �������� �� ������ ������
    void PopFront() noexcept {
        assert(!IsEmpty());

        Node* tmp = head_.next_node->next_node;
        delete head_.next_node;
        head_.next_node = tmp;
        --size_;
    }

    // ������� ���������� � ����� ������
    // ��� �� ������� ������, �� ����� ����� (� ��������� ������� �� ������������)
    void PushBack(const Type& value) {
        Node* back = &head_;
        while (back->next_node) { back = back->next_node;}

        back->next_node = new Node(value, nullptr);
        ++size_;
    }

    // ������� �������� �� ����� ������
    // ��� �� ������� ������, �� ����� ����� (� ��������� ������� �� ������������)
    void PopBack() noexcept  {
        assert(!IsEmpty());

        Node* pre_back = &head_;
        while (pre_back->next_node->next_node) { pre_back = pre_back->next_node; }

        delete pre_back->next_node;
        pre_back->next_node = nullptr;
        --size_;
    }

    // ������� ���������� ����� ��������, �� ������� ��������� ��������
    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        assert(pos.node_);

        pos.node_->next_node = new Node(value, pos.node_->next_node);
        ++size_;
        return Iterator(pos.node_->next_node);
    }

    // ������� �������� ����� ��������, �� ������� ��������� ��������
    Iterator EraseAfter(ConstIterator pos) noexcept {
        assert(pos.node_);
        assert(pos.node_->next_node);

        Node* tmp = pos.node_->next_node->next_node;
        delete pos.node_->next_node;
        pos.node_->next_node = tmp;
        --size_;

        return Iterator(tmp);
    }

    // ������� ������
    void Clear() noexcept {
        while (head_.next_node) {
            Node* tmp = head_.next_node->next_node;
            delete head_.next_node;
            head_.next_node = tmp;
        }

        size_ = 0u;
    }

private:
    Node head_;        // ��������� ����
    size_t size_ = 0u; // ������ ������

    // ������� ��� ���������� ������ copy-and-swap � ������������: ������ ������, ������������������
    // ���������� � ��������� [begin; end) � ������ ��� ������� � �������
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

// ������� ������ ������� swap(lhs, rhs)
template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

// �������� ��������� ������� "=="
template <typename Type>
bool operator == (const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
}

// �������� ��������� ������� "<"
template <typename Type>
bool operator < (const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
}

// ����������� �� "==" � "<" ��������� ��������� �������
template <typename Type>
bool operator != (const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) { return !(lhs == rhs); }

template <typename Type>
bool operator >  (const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) { return rhs < lhs; }

template <typename Type>
bool operator <= (const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) { return !(rhs < lhs); }

template <typename Type>
bool operator >= (const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) { return !(lhs < rhs); }

// �������� "<<" ��� ������ ����������� ������ � �����
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