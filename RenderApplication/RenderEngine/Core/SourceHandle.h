#pragma once

#include <glad/glad.h>

struct SourceHandleWrapperRecycle {};
struct SourceHandleWrapperReserved {};

template <typename ORIGIN, typename TARGET, typename TYPE = SourceHandleWrapperRecycle>
class SourceHandleWrapper
{
public:
    // 1. 构造函数：支持 SourceHandleWrapper<int> a = 10;
    template <typename... Args>
    SourceHandleWrapper(Args&&... args) : m_Origin(new ORIGIN(std::forward<Args>(args)...)),
        m_Target(new TARGET(*m_Origin)), m_ref_count(new unsigned int(1))
    {
        if (std::is_same<TYPE, SourceHandleWrapperRecycle>::value) {
            LocalFree(&m_Origin);
        }
    }

    // 默认构造
    SourceHandleWrapper()
        : m_Origin(nullptr), m_Target(nullptr), m_ref_count(nullptr)
    {
    }

    // 2. 拷贝构造
    SourceHandleWrapper(const SourceHandleWrapper<ORIGIN, TARGET>& other)
        : m_Origin(other.m_Origin), m_Target(other.m_Target), m_ref_count(other.m_ref_count)
    {
        if (m_ref_count)
        {
            (*m_ref_count)++;
        }
    }

    // 3. 移动构造
    SourceHandleWrapper(SourceHandleWrapper<ORIGIN, TARGET>&& other) noexcept
        : m_Origin(other.m_Origin), m_Target(other.m_Target), m_ref_count(other.m_ref_count)
    {
        other.m_Origin = nullptr;
        other.m_Target = nullptr;
        other.m_ref_count = nullptr;
    }

    // 4. 隐式类型转换：解决“无法从 SourceHandleWrapper 转换为 T”的问题
    // 支持：int x = a;
    operator TARGET() const
    {
        return m_Target ? *m_Target : TARGET();
    }

    // 支持：T& ref = a; 修改引用即修改内部值
    operator TARGET& ()
    {
        return *m_Target;
    }
    ORIGIN* getOrigin()
    {
        return m_Origin;
    }

    // 拷贝赋值
    SourceHandleWrapper<ORIGIN, TARGET>& operator=(const SourceHandleWrapper<ORIGIN, TARGET>& other)
    {
        if (this != &other)
        {
            release();
            m_Origin = other.m_Origin;
            m_Target = other.m_Target;
            m_ref_count = other.m_ref_count;
            if (m_ref_count)
            {
                (*m_ref_count)++;
            }
        }
        return *this;
    }

    ~SourceHandleWrapper()
    {
        release();
    }

    TARGET& operator*() const { return *m_Target; }
    TARGET* operator->() const { return m_Target; }
    unsigned int use_count() const { return m_ref_count ? *m_ref_count : 0; }

private:
    void release()
    {
        if (m_ref_count && --(*m_ref_count) == 0)
        {
            LocalFree(&m_Origin);
            LocalFree(&m_Target);
            LocalFree(&m_ref_count);
        }
        m_Origin = nullptr;
        m_Target = nullptr;
        m_ref_count = nullptr;
    }
    template <typename T>
    void LocalFree(T** p)
    {
        if (nullptr != p && nullptr != *p) {
            delete* p;
            *p = nullptr;
        }
    }

private:
    ORIGIN* m_Origin;
    TARGET* m_Target;
    unsigned int* m_ref_count;
};

template <typename T>
class SourceHandle
{
public:
    // 1. 构造函数：支持 SourceHandle<int> a = 10;
    template <typename... Args>
    SourceHandle(Args&&... args)
        : m_Value(new T(std::forward<Args>(args)...)),
        m_ref_count(new int(1))
    {
    }

    // 默认构造
    SourceHandle()
        : m_Value(new T()), m_ref_count(new int(1))
    {
    }

    // 2. 拷贝构造
    SourceHandle(const SourceHandle<T>& other)
        : m_Value(other.m_Value), m_ref_count(other.m_ref_count)
    {
        if (m_ref_count)
        {
            (*m_ref_count)++;
        }
    }

    // 4. 隐式类型转换：解决“无法从 SourceHandle 转换为 T”的问题
    // 支持：int x = a;
    operator T() const
    {
        return m_Value ? *m_Value : T();
    }

    // 支持：T& ref = a; 修改引用即修改内部值
    operator T& ()
    {
        return *m_Value;
    }

    // 拷贝赋值
    SourceHandle<T>& operator=(const SourceHandle<T>& other)
    {
        if (this != std::addressof(other))
        {
            release();
            m_Value = other.m_Value;
            m_ref_count = other.m_ref_count;
            if (m_ref_count)
            {
                (*m_ref_count)++;
            }
        }
        return *this;
    }

    auto operator&() const -> decltype(&std::declval<const T&>())
    {
        return &(*m_Value);
    }
    // 针对指针的穿透设计（解决 unsigned int* p = &vao;）
    auto operator&() -> decltype(&std::declval<T&>())
    {
        return &(*m_Value);
    }
    // 大胆的设计：支持隐式转换为「任何」内部对象 T 支持转换到的类型
    template <typename U>
    operator U() const
    {
        // 如果 T 可以转换为 U（比如 IDManager 转 unsigned int），则直接转换
        return m_Value ? static_cast<U>(*m_Value) : U();
    }

    ~SourceHandle()
    {
        release();
    }

    T& operator*() const { return *m_Value; }
    T* operator->() const { return m_Value; }
    int use_count() const { return m_ref_count ? *m_ref_count : 0; }

private:
    void release()
    {
        if (m_ref_count && --(*m_ref_count) == 0)
        {
            LocalFree(m_Value);
            LocalFree(m_ref_count);
        }
        m_Value = nullptr;
        m_ref_count = nullptr;
    }
    template <typename type>
    void LocalFree(type* p)
    {
        if (nullptr != p) {
            delete p;
            p = nullptr;
        }
    }
private:
    T* m_Value;
    int* m_ref_count;
};

struct IDManagerVAO {};
struct IDManagerEBO {};
struct IDManagerVBO {};
struct IDManagerUBO {};
struct IDManagerFBO {};
struct IDManagerRBO {};
struct IDManagerTEX {}; // 纹理
struct IDManagerSHD {}; // 着色器
struct IDManagerSHDP {}; // 着色器Program

template<typename T>
class IDManager
{
public:
    // 1. 默认构造
    IDManager() :m_ID(0)
    {

    }
    // 2. 隐式构造函数：允许 IDManager a = 10;
    IDManager(unsigned int id): m_ID(id)
    {

    }
    // 隐式构造函数：IDManager a = 10; IDManager b = a;
    IDManager(const IDManager& other) : m_ID(other.m_ID)
    {

    }
    // 3. 赋值运算符重载：允许 a = 20;
    IDManager& operator=(unsigned int id) 
    {
        this->m_ID = id;
        return *this;
    }
    // 4. 重载取地址运算符 & ：允许 int* p = &IDManager实例;
    // 注意：OpenGL 通常需要 GLuint*，所以返回 unsigned int*
    unsigned int* operator &()
    {
        return &m_ID;
    }
    unsigned int* operator &() const
    {
        return &m_ID;
    }
    // 5. (可选) 重载隐式类型转换：允许像使用 int 一样使用对象
    operator unsigned int() const {
        return m_ID;
    }
    IDManager operator = (const IDManager& other)
    {
        this->m_ID = other.m_ID;
        return *this;
    }
    
    ~IDManager()
    {
        Release();
    }
private:
    void Release()
    {
        if (std::is_same<T, IDManagerVAO>::value) {
            ReleaseVAO();
        }
        else if (std::is_same<T, IDManagerEBO>::value ||
            std::is_same<T, IDManagerVBO>::value || 
            std::is_same<T, IDManagerUBO>::value ) {
            ReleaseEBO();
        }
        else if (std::is_same<T, IDManagerFBO>::value) {
            ReleaseFBO();
        }
        else if (std::is_same<T, IDManagerRBO>::value) {
            ReleaseRBO();
        }
        else if (std::is_same<T, IDManagerTEX>::value) {
            ReleaseTEX();
        }
        else if (std::is_same<T, IDManagerSHD>::value) {
            ReleaseSHD();
        }
        else if (std::is_same<T, IDManagerSHDP>::value) {
            ReleaseSHDP();
        }
        m_ID = 0;
    }
private:
    // 回收 VAO (Vertex Array Object)
    void ReleaseVAO()
    {
        if (0 != m_ID && glIsVertexArray(m_ID)) {
            glDeleteVertexArrays(1, &m_ID);
        }
    }
    // 回收 VBO / EBO / UBO (Buffers)
    void ReleaseEBO()
    {
        if (0 != m_ID && glIsBuffer(m_ID)) {
            glDeleteBuffers(1, &m_ID);
        }
    }
    void ReleaseFBO()
    {
        if (0 != m_ID && glIsFramebuffer(m_ID)) {
            glDeleteFramebuffers(1, &m_ID);
        }
    }
    void ReleaseRBO()
    {
        if (0 != m_ID && glIsRenderbuffer(m_ID)) {
            glDeleteRenderbuffers(1, &m_ID);
        }
    }
    // 回收纹理
    void ReleaseTEX()
    {
        if (0 != m_ID && glIsTexture(m_ID)) {
            glDeleteTextures(1, &m_ID);
        }
    }
    // 回收着色器（片段和顶点）
    void ReleaseSHD()
    {
        // 如果你手动管理着色器片段 (Shader Object)
        if (0 != m_ID && glIsShader(m_ID)) {
            glDeleteShader(m_ID);
        }
    }
    void ReleaseSHDP()
    {
        // 回收着色器程序 (Program)
        if (0 != m_ID && glIsProgram(m_ID)) {
            glDeleteProgram(m_ID);
        }
    }
private:
    unsigned int    m_ID;
};