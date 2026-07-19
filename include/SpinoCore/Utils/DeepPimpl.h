#pragma once
#include <memory>

template<typename T>
class DeepPimpl {
public:
    explicit DeepPimpl(std::unique_ptr<T> p) : mPtr(std::move(p)) {}

    T* operator->() { return mPtr.get(); }
    const T* operator->() const { return mPtr.get(); }

    T& operator*() { return *mPtr; }
    const T& operator*() const { return *mPtr; }

private:
    std::unique_ptr<T> mPtr;
};
