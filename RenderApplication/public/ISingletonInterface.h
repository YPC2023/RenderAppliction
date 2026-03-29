#pragma once


template <typename T>
class ISingletonInterface
{
public:
    // 获取单例的全局访问点
    static T& GetInstance() {
        // C++11 后，静态局部变量是线程安全的
        static T instance;
        return instance;
    }

    // 禁止拷贝和赋值
    ISingletonInterface(const ISingletonInterface&) = delete;
    ISingletonInterface& operator=(const ISingletonInterface&) = delete;

protected:
    // 构造函数设为 protected，防止外部直接实例化
    ISingletonInterface() = default;
    virtual ~ISingletonInterface() = default;
};

