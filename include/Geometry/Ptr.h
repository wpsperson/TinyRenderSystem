#pragma once

namespace geom
{
template<typename T>
class  Ptr
{
public:
    Ptr() : m_ptr(nullptr)
    {
    }

    ~Ptr()
    {
        if (!m_ptr)
        {
            return;
        }
        if (0 == m_ptr->decrease())
        {
            delete m_ptr;
            m_ptr = nullptr;
        }
    }

    Ptr(T* pointer) : m_ptr(pointer)
    {
        if (m_ptr)
        {
            m_ptr->increase();
        }
    }

    Ptr(const Ptr& rhs) : m_ptr(rhs.m_ptr)
    {
        if (m_ptr)
        {
            m_ptr->increase();
        }
    }

    Ptr& operator = (const Ptr& rhs)
    {
        if (m_ptr == rhs.m_ptr)
        {
            return *this;
        }

        T* old = m_ptr;
        m_ptr = rhs.m_ptr;
        if (m_ptr)
        {
            m_ptr->increase();
        }

        if (old)
        {
            if (0 == old->decrease())
            {
                delete old;
            }
        }
        return *this;
    }

    Ptr& operator = (T* pointer)
    {
        if (m_ptr == pointer)
        {
            return *this;
        }

        T* old = m_ptr;
        m_ptr = pointer;
        if (m_ptr)
        {
            m_ptr->increase();
        }

        if (old)
        {
            if (0 == old->decrease())
            {
                delete old;
            }
        }
        return *this;
    }

    operator T* () const { return m_ptr; }

    T& operator*() const { return *m_ptr; }

    T* operator->() const { return m_ptr; }

    T* get() const { return m_ptr; }

    bool valid() const { return m_ptr != nullptr; }

    T* release()
    {
        T* old = m_ptr;

        if (m_ptr)
        {
            m_ptr->decrease();
        }

        m_ptr = nullptr;

        return old;
    }

    bool operator == (const Ptr& rhs) const { return (m_ptr == rhs.m_ptr); }

    bool operator == (const T* pointer) const { return (m_ptr == pointer); }

    bool operator != (const Ptr& rhs) const { return (m_ptr != rhs._ptr); }

    bool operator != (const T* pointer) const { return (m_ptr != pointer); }

private:
    T* m_ptr;
};

}