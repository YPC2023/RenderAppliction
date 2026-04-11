#pragma once
#include <functional>
#include <utility>

template<typename T>
class VarWatcher {
public:
    // 回调函数类型：参数为旧值和新值
    using Callback = std::function<void(const T& old_value, const T& new_value)>;

    // 构造函数：初始化变量值，并可选地设置回调
    explicit VarWatcher(const T& initial_value, Callback callback = nullptr)
        : value_(initial_value), callback_(std::move(callback)) {
    }

    // 设置新值（左值版本），若值改变则触发回调
    void set(const T& new_value) {
        if (value_ == new_value) return;
        T old_value = std::move(value_);
        value_ = new_value;
        if (callback_) {
            callback_(old_value, value_);
        }
    }

    // 获取当前值（只读访问）
    const T& get() const {
        return value_;
    }

    // 设置回调函数
    void setCallback(Callback callback) {
        callback_ = std::move(callback);
    }

private:
    T value_;
    Callback callback_;
};