#ifndef ANY_H
#define ANY_H

#include <memory>
#include <typeindex>


struct Any
{
    Any(void) : typeIndex_(std::type_index(typeid(void))) {}
    Any(const Any& other) : ptr_(other.Clone()), typeIndex_(other.typeIndex_) {}
    Any(Any&& other) : ptr_(std::move(other.ptr_)), typeIndex_(other.typeIndex_) {}

    template<typename U, typename = typename std::enable_if<!std::is_same<typename std::decay<U>::type, Any>::value, U>::type>
    Any(U && value) : ptr_(new Derived< typename std::decay<U>::type >(std::forward<U>(value))),
        typeIndex_(std::type_index(typeid(typename std::decay<U>::type))) {}

    bool IsNull() const { return !bool(ptr_); }

    template <typename U>
    bool Is() const
    {
        return typeIndex_ == std::type_index(typeid(U));
    }

    template <typename U>
    U& AnyCast()
    {
        if (!Is<U>())
        {
            throw std::bad_cast();
        }

        auto derived = dynamic_cast<Derived<U>*>(ptr_.get());
        return derived->value_;
    }

    Any& operator=(const Any& other)
    {
        if (ptr_ == other.ptr_)
        {
            return *this;
        }
    
        ptr_ = other.Clone();
        typeIndex_ = other.typeIndex_;
        return *this;
    }

private:
    struct Base;
    using BasePtr = std::unique_ptr<Base>;

    struct Base
    {
        virtual ~Base() {}
        virtual BasePtr Clone() const = 0;
    };

    template <typename T>
    struct Derived : Base
    {
        template <typename U>
        Derived(U && value) : value_(std::forward<U>(value)) {}

        BasePtr Clone() const
        {
            return BasePtr(new Derived<T>(value_));
        }
    
        T value_;
    };

    BasePtr Clone() const
    {
        if (ptr_ != nullptr)
            return ptr_->Clone();

        return nullptr;
    }

    BasePtr ptr_;
    std::type_index typeIndex_;
};

#endif