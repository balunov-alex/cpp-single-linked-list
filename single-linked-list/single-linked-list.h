#pragma once

#include <cassert>
#include <cstddef>
#include <iterator>
#include <string>
#include <utility>

template <typename Type>
class SingleLinkedList {
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value;
        Node* next_node = nullptr;
    };
    
    Node head_;
    size_t size_ = 0u;
    
    template <typename Container>
    void CreateSingleLinkedList(const Container& container) {
        SingleLinkedList container_copy;
        auto insert_after_pos = container_copy.before_begin();
        for (auto it = container.begin(); it != container.end(); ++it) {
            insert_after_pos = container_copy.InsertAfter(insert_after_pos, *it);
        }
        swap(container_copy);
    } 

    template <typename ValueType>
    class BasicIterator {
    
        friend class SingleLinkedList;
        explicit BasicIterator<ValueType>(Node* node)
            : node_(node)
        {
        }
        
        Node* node_ = nullptr;

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType*;
        using reference = ValueType&;

        BasicIterator() = default;

        BasicIterator(const BasicIterator<Type>& other) noexcept 
            : node_(other.node_)
        {   
        }

        BasicIterator<ValueType>& operator=(const BasicIterator<ValueType>& rhs) = default;

        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            return node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            return !(node_ == rhs.node_);
        }

        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            return node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
            return !(node_ == rhs.node_);
        }
        
        BasicIterator<ValueType>& operator++() noexcept {
            assert(node_);
            node_ = node_->next_node;
            return *this;
            }

        BasicIterator<ValueType> operator++(int) noexcept {
            BasicIterator<ValueType> ex_it(*this);
            ++(*this);
            return ex_it;
        }
        
        [[nodiscard]] reference operator*() const noexcept {
            assert(node_);
            return node_->value;  
        }

        [[nodiscard]] pointer operator->() const noexcept {
            assert(node_);
            return &(node_->value);
        }
    };    

public:
    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using Iterator = BasicIterator<Type>;
    using ConstIterator = BasicIterator<const Type>;
    
    SingleLinkedList() = default;

    SingleLinkedList(std::initializer_list<Type> values) {
        assert(size_ == 0 && head_.next_node == nullptr);
        CreateSingleLinkedList(values);
    }

    SingleLinkedList(const SingleLinkedList& other) {
        assert(size_ == 0 && head_.next_node == nullptr);
        CreateSingleLinkedList(other);
    }   

    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        if (this != &rhs) {
            auto rhs_copy(rhs);
            swap(rhs_copy);
        }
        return *this;
    }

    void swap(SingleLinkedList& other) noexcept {
        std::swap(size_, other.size_);
        std::swap(head_.next_node, other.head_.next_node);
    }
    
    ~SingleLinkedList() {
        Clear();
    }

    void PushFront(const Type& value) {
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }
    
    void PopFront() noexcept {
        assert(size_ != 0 && head_.next_node != nullptr);
        Node* new_first_node = head_.next_node->next_node;
        delete head_.next_node;
        head_.next_node = new_first_node;
        --size_;
    }
    
    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        assert(pos.node_);
        pos.node_->next_node = new Node(value, pos.node_->next_node);
        ++size_;
        return Iterator(pos.node_->next_node);
    }
    
    Iterator EraseAfter(ConstIterator pos) noexcept {
        assert(size_ != 0 && pos.node_ && pos.node_->next_node);
        Node* ptr_on_elem_after_deleted = pos.node_->next_node->next_node;
        delete pos.node_->next_node;
        pos.node_->next_node = ptr_on_elem_after_deleted;
        --size_;
        return Iterator(ptr_on_elem_after_deleted);
    }    

    void Clear() noexcept {
        while (head_.next_node) {
            Node* deleted_node = head_.next_node;
            head_.next_node = head_.next_node->next_node;
            delete deleted_node;
        }
        size_ = 0u;
    }
    
    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    [[nodiscard]] bool IsEmpty() const noexcept {
        return !size_;
    }

    [[nodiscard]] Iterator begin() noexcept {
        return Iterator(head_.next_node);
    }

    [[nodiscard]] Iterator end() noexcept {
        return Iterator(nullptr);
    }

    [[nodiscard]] ConstIterator begin() const noexcept {
        return cbegin();
    }

    [[nodiscard]] ConstIterator end() const noexcept {
        return cend();
    }

    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return ConstIterator(head_.next_node);
    }

    [[nodiscard]] ConstIterator cend() const noexcept {
        return ConstIterator(nullptr);
    }
    
    [[nodiscard]] Iterator before_begin() noexcept {
        return Iterator(&head_);
    }

    [[nodiscard]] ConstIterator before_begin() const noexcept {
        return cbefore_begin();
    }
    
    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        return ConstIterator(const_cast<Node*>(&head_));
    }    
};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return lhs.GetSize() == rhs.GetSize() && std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(rhs < lhs);
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return rhs < lhs;
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs < rhs);
}
