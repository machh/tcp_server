#ifndef _DEFER_H_
#define _DEFER_H_

#include <functional>
// 基于用法:
//
//  FILE* fp = fopen("foo.txt", "rt");
//  if(fp == NULL) return false;
//  defer([&](){ printf("fclose(fp)\n"); fclose(fp); });

// 持有闭包函数
class _DeferredAction {
private:
    std::function<void()> func_;

    template<typename T>
    friend _DeferredAction _DeferredActionCtor(T&& p);

    template<typename T>
    _DeferredAction(T&& p): func_(std::bind(std::forward<T>(p))) {}

    _DeferredAction();
    _DeferredAction(_DeferredAction const&);
    _DeferredAction& operator=(_DeferredAction const&);
    _DeferredAction& operator=(_DeferredAction&&);

public:
    _DeferredAction(_DeferredAction&& other):
            func_(std::forward<std::function<void()>>(other.func_)) {
        other.func_ = nullptr;
    }
    ~_DeferredAction() {
        if(func_) { func_(); }
    }
};

template<typename T>
_DeferredAction _DeferredActionCtor(T&& p) {
    return _DeferredAction(std::forward<T>(p));
}

#endif  // _DEFER_H_