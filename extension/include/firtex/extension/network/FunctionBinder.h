//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (http://sourceforge.net/projects/firtex)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software.
//
// Author  : Ruijie Guo
// Email   : ruijieguo@gmail.com
// Created : 2011-04-27 21:41:14

#ifndef __FX_FUNCTIONBINDER_H
#define __FX_FUNCTIONBINDER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"

FX_NS_DEF(network);


template<class T> 
class ReferenceWrapper
{ 
public:
    typedef T type;

    explicit ReferenceWrapper(T& t): m_t(&t) {}

    operator T& () const { return *m_t; }

    T& get() const { return *m_t; }

    T* gem_tpointer() const { return m_t; }

private:

    T* m_t;
};

template<class T>
inline ReferenceWrapper<T> const ref(T & t)
{ 
    return ReferenceWrapper<T>(t);
}

template<class T> 
inline ReferenceWrapper<T const> const cref(T const & t)
{
    return ReferenceWrapper<T const>(t);
}

#define DECLARE_CLASS_AND_BINDER0(baseClass)                            \
    template <typename T>                                               \
    class DerivedCallback0_##baseClass;                                 \
    class baseClass                                                     \
    {                                                                   \
    public:                                                             \
        virtual ~baseClass() {}                                         \
        virtual void operator () () {};                                 \
    };                                                                  \
    template <class T> class DerivedCallback0_##baseClass : public baseClass \
    {                                                                   \
    public:                                                             \
        typedef void (T::*Callback)();                                  \
    public:                                                             \
        DerivedCallback0_##baseClass(T* pObject, Callback method)       \
            : m_pObject(pObject)                                        \
            , m_fnMethod(method)                                        \
        {}                                                              \
        DerivedCallback0_##baseClass(const DerivedCallback0_##baseClass& src) \
            : m_pObject(src.m_pObject)                                  \
            , m_fnMethod(src.m_fnMethod)                                \
        {}                                                              \
    public:                                                             \
        inline void operator () ()                                      \
        {                                                               \
            (m_pObject->*m_fnMethod)();                                 \
        }                                                               \
    private:                                                            \
        T* m_pObject;                                                   \
        Callback m_fnMethod;                                            \
    };                                                                  \
    class baseClass##Binder                                             \
    {                                                                   \
    public:                                                             \
        template <typename T>                                           \
            static DerivedCallback0_##baseClass<T>*                     \
            bind(T* pObj, typename DerivedCallback0_##baseClass<T>::Callback method) \
        {                                                               \
            return new DerivedCallback0_##baseClass<T>(pObj, method);   \
        }                                                               \
    }

#define DECLARE_CLASS_AND_BINDER1(baseClass, argType)                   \
    template <typename T>                                               \
    class DerivedCallback1_##baseClass;                                 \
    class baseClass                                                     \
    {                                                                   \
    public:                                                             \
        virtual ~baseClass() {}                                         \
        virtual void operator () (argType a) {};                        \
    };                                                                  \
    template <class T> class DerivedCallback1_##baseClass : public baseClass \
    {                                                                   \
    public:                                                             \
        typedef void (T::*Callback)(argType arg);                       \
    public:                                                             \
        DerivedCallback1_##baseClass(T* pObject, Callback method)       \
            : m_pObject(pObject)                                        \
            , m_fnMethod(method)                                        \
        {}                                                              \
        DerivedCallback1_##baseClass(const DerivedCallback1_##baseClass& src) \
            : m_pObject(src.m_pObject)                                  \
            , m_fnMethod(src.m_fnMethod)                                \
        {}                                                              \
    public:                                                             \
        inline void operator () (argType a)                             \
        {                                                               \
            (m_pObject->*m_fnMethod)(a);                                \
        }                                                               \
    private:                                                            \
        T* m_pObject;                                                   \
        Callback m_fnMethod;                                            \
    };                                                                  \
    class baseClass##Binder                                             \
    {                                                                   \
    public:                                                             \
        template <typename T>                                           \
            static DerivedCallback1_##baseClass<T>*                     \
            bind(T* pObj, typename DerivedCallback1_##baseClass<T>::Callback method) \
        {                                                               \
            return new DerivedCallback1_##baseClass<T>(pObj, method);   \
        }                                                               \
    }


#define DECLARE_CLASS_AND_BINDER2(baseClass, arg1Type, arg2Type)        \
    class baseClass                                                     \
    {                                                                   \
    public:                                                             \
        virtual ~baseClass() {}                                         \
        virtual void operator () (arg1Type a1, arg2Type a2) {};         \
    };                                                                  \
    template <class T> class DerivedCallback2_##baseClass : public baseClass \
    {                                                                   \
    public:                                                             \
        typedef void (T::*Callback)(arg1Type a1, arg2Type a2);          \
    public:                                                             \
        DerivedCallback2_##baseClass(T* pObject, Callback method)       \
            : m_pObject(pObject)                                        \
            , m_fnMethod(method)                                        \
        {}                                                              \
        DerivedCallback2_##baseClass(const DerivedCallback2_##baseClass& src) \
            : m_pObject(src.m_pObject)                                  \
            , m_fnMethod(src.m_fnMethod)                                \
        {}                                                              \
    public:                                                             \
        inline void operator () (arg1Type a1, arg2Type a2)              \
        {                                                               \
            (m_pObject->*m_fnMethod)(a1, a2);                           \
        }                                                               \
    private:                                                            \
        T* m_pObject;                                                   \
        Callback m_fnMethod;                                            \
    };                                                                  \
    class baseClass##Binder                                             \
    {                                                                   \
    public:                                                             \
        template <typename T>                                           \
            static DerivedCallback2_##baseClass<T>*                     \
            bind(T* pObj, typename DerivedCallback2_##baseClass<T>::Callback method) \
        {                                                               \
            return new DerivedCallback2_##baseClass<T>(pObj, method);   \
        }                                                               \
    }

#define DECLARE_CLASS_AND_SHARED_BINDER2(baseClass, arg1Type, arg2Type) \
    class baseClass                                                     \
    {                                                                   \
    public:                                                             \
        virtual ~baseClass() {}                                         \
        virtual void operator () (arg1Type a1, arg2Type a2) {};         \
    };                                                                  \
    template <class T> class DerivedCallback2_##baseClass : public baseClass \
    {                                                                   \
    public:                                                             \
        typedef void (T::*Callback)(arg1Type a1, arg2Type a2);          \
    public:                                                             \
        DerivedCallback2_##baseClass(T* pObject, Callback method)       \
            : m_pObject(pObject)                                        \
            , m_fnMethod(method)                                        \
        {}                                                              \
        DerivedCallback2_##baseClass(const DerivedCallback2_##baseClass& src) \
            : m_pObject(src.m_pObject)                                  \
            , m_fnMethod(src.m_fnMethod)                                \
        {}                                                              \
    public:                                                             \
        inline void operator () (arg1Type a1, arg2Type a2)              \
        {                                                               \
            (m_pObject.get()->*m_fnMethod)(a1, a2);                     \
        }                                                               \
    private:                                                            \
        DEFINE_TYPED_PTR(T);                                            \
        ObjPtr m_pObject;                                               \
        Callback m_fnMethod;                                            \
    };                                                                  \
    class baseClass##Binder                                             \
    {                                                                   \
    public:                                                             \
        template <typename T>                                           \
            static DerivedCallback2_##baseClass<T>*                     \
            bind(T* pObj, typename DerivedCallback2_##baseClass<T>::Callback method) \
        {                                                               \
            return new DerivedCallback2_##baseClass<T>(pObj, method);   \
        }                                                               \
    }


#define DECLARE_CLASS_AND_BINDER3(baseClass, arg1Type, arg2Type, arg3Type) \
    class baseClass                                                     \
    {                                                                   \
    public:                                                             \
        virtual ~baseClass() {}                                         \
        virtual void operator () (arg1Type a1, arg2Type a2, arg3Type a3) {}; \
    };                                                                  \
    template <class T> class DerivedCallback3_##baseClass : public baseClass \
    {                                                                   \
    public:                                                             \
        typedef void (T::*Callback)(arg1Type a1, arg2Type a2, arg3Type a3); \
    public:                                                             \
        DerivedCallback3_##baseClass(T* pObject, Callback method)       \
            : m_pObject(pObject)                                        \
            , m_fnMethod(method)                                        \
        {}                                                              \
        DerivedCallback3_##baseClass(const DerivedCallback3_##baseClass& src) \
            : m_pObject(src.m_pObject)                                  \
            , m_fnMethod(src.m_fnMethod)                                \
        {}                                                              \
    public:                                                             \
        virtual void operator () (arg1Type a1, arg2Type a2, arg3Type a3) \
        {                                                               \
            (m_pObject->*m_fnMethod)(a1, a2, a3);                       \
        }                                                               \
    private:                                                            \
        T* m_pObject;                                                   \
        Callback m_fnMethod;                                            \
    };                                                                  \
    class baseClass##Binder                                             \
    {                                                                   \
    public:                                                             \
        template <typename T>                                           \
            static DerivedCallback3_##baseClass<T>*                     \
            bind(T* pObj, typename DerivedCallback3_##baseClass<T>::Callback method) \
        {                                                               \
            return new DerivedCallback3_##baseClass<T>(pObj, method);   \
        }                                                               \
    }


#define DECLARE_BINDER1(thisClass, handleClass, argType)                \
    template <class T>                                                  \
    class Typed##handleClass                                            \
    {                                                                   \
    public:                                                             \
        typedef void (T::*Callback)(argType a1);                        \
    public:                                                             \
        Typed##handleClass(T* pObject, Callback method, argType a1)     \
            : m_pObject(pObject)                                        \
            , m_fnMethod(method)                                        \
            , m_arg1(a1)                                                \
        {}                                                              \
        Typed##handleClass(const Typed##handleClass& src)               \
            : m_pObject(src.m_pObject)                                  \
            , m_fnMethod(src.m_fnMethod)                                \
            , m_arg1(src.m_arg1)                                        \
        {}                                                              \
    public:                                                             \
        inline void operator()(argType a1)                              \
        {                                                               \
            (m_pObject->*m_fnMethod)(a1);                               \
        }                                                               \
        inline void operator()()                                        \
        {                                                               \
            (m_pObject->*m_fnMethod)(m_arg1);                           \
        }                                                               \
    private:                                                            \
        T* m_pObject;                                                   \
        Callback m_fnMethod;                                            \
        argType m_arg1;                                                 \
    };                                                                  \
    typedef Typed##handleClass<thisClass> handleClass;                   \
    class handleClass##Binder                                           \
    {                                                                   \
    public:                                                             \
        template <typename T>                                           \
            static thisClass::Typed##handleClass<T>                     \
            bind(T* pObj, typename thisClass::Typed##handleClass<T>::Callback method, argType a1) \
        {                                                               \
            return thisClass::Typed##handleClass<T>(pObj, method, a1);  \
        }                                                               \
    }

#define DECLARE_SHARED_BINDER1(thisClass, handleClass, argType)         \
    template <class T>                                                  \
    class Typed##handleClass                                            \
    {                                                                   \
    public:                                                             \
        typedef void (T::*Callback)(argType a1);                        \
        DEFINE_TYPED_PTR(T);                                            \
    public:                                                             \
        Typed##handleClass(T* pObject, Callback method, argType a1)     \
            : m_pObject(pObject)                                        \
            , m_fnMethod(method)                                        \
            , m_arg1(a1)                                                \
        {}                                                              \
        Typed##handleClass(const ObjPtr& pObject, Callback method, argType a1) \
            : m_pObject(pObject)                                        \
            , m_fnMethod(method)                                        \
            , m_arg1(a1)                                                \
        {}                                                              \
        Typed##handleClass(const Typed##handleClass& src)               \
            : m_pObject(src.m_pObject)                                  \
            , m_fnMethod(src.m_fnMethod)                                \
            , m_arg1(src.m_arg1)                                        \
        {}                                                              \
    public:                                                             \
        inline void operator()(argType a1)                              \
        {                                                               \
            (m_pObject.get()->*m_fnMethod)(a1);                         \
        }                                                               \
        inline void operator()()                                        \
        {                                                               \
            (m_pObject.get()->*m_fnMethod)(m_arg1);                     \
        }                                                               \
    private:                                                            \
        ObjPtr m_pObject;                                               \
        Callback m_fnMethod;                                            \
        argType m_arg1;                                                 \
    };                                                                  \
    typedef Typed##handleClass<thisClass> handleClass;                   \
    class handleClass##Binder                                           \
    {                                                                   \
    public:                                                             \
        template <typename T>                                           \
            inline static thisClass::Typed##handleClass<T>              \
            bind(T* pObj, typename thisClass::Typed##handleClass<T>::Callback method, argType a1) \
        {                                                               \
            return thisClass::Typed##handleClass<T>(pObj, method, a1);  \
        }                                                               \
        template <typename T>                                           \
            inline static thisClass::Typed##handleClass<T>              \
            bind(const FX_NS(common)::SharedPtr<T>& pObj, typename thisClass::Typed##handleClass<T>::Callback method, argType a1) \
        {                                                               \
            return thisClass::Typed##handleClass<T>(pObj, method, a1);  \
        }                                                               \
    }

#define DECLARE_BINDER2(thisClass, handleClass, arg1Type, arg2Type)     \
    template <class T>                                                  \
    class Typed##handleClass                                            \
    {                                                                   \
    public:                                                             \
        typedef void (T::*Callback)(arg1Type a1, arg2Type a2);          \
    public:                                                             \
        Typed##handleClass(T* pObject, Callback method, arg1Type a1, arg2Type a2) \
            : m_pObject(pObject)                                        \
            , m_fnMethod(method)                                        \
            , m_arg1(a1)                                                \
            , m_arg2(a2)                                                \
        {}                                                              \
        Typed##handleClass(const Typed##handleClass& src)               \
            : m_pObject(src.m_pObject)                                  \
            , m_fnMethod(src.m_fnMethod)                                \
            , m_arg1(src.m_arg1)                                        \
            , m_arg2(src.m_arg2)                                        \
        {}                                                              \
    public:                                                             \
        inline void operator()(const asio::error_code&)                 \
        {                                                               \
            (m_pObject->*m_fnMethod)(m_arg1, m_arg2);                   \
        }                                                               \
        inline void operator()(arg1Type a1, arg2Type a2)                \
        {                                                               \
            (m_pObject->*m_fnMethod)(a1, a2);                           \
        }                                                               \
        inline void operator()()                                        \
        {                                                               \
            (m_pObject->*m_fnMethod)(m_arg1, m_arg2);                   \
        }                                                               \
    private:                                                            \
        T* m_pObject;                                                   \
        Callback m_fnMethod;                                            \
        arg1Type m_arg1;                                                \
        arg2Type m_arg2;                                                \
    };                                                                  \
    typedef Typed##handleClass<thisClass> handleClass;                   \
    class handleClass##Binder                                           \
    {                                                                   \
    public:                                                             \
        template <typename T>                                           \
            static thisClass::Typed##handleClass<T>                     \
            bind(T* pObj, typename thisClass::Typed##handleClass<T>::Callback method, arg1Type a1, arg2Type a2) \
        {                                                               \
            return thisClass::Typed##handleClass<T>(pObj, method, a1, a2); \
        }                                                               \
    }

#define DECLARE_SHARED_BINDER2(thisClass, handleClass, arg1Type, arg2Type) \
    template <class T>                                                  \
    class Typed##handleClass                                            \
    {                                                                   \
    public:                                                             \
        typedef void (T::*Callback)(arg1Type a1, arg2Type a2);          \
    public:                                                             \
        Typed##handleClass(T* pObject, Callback method, arg1Type a1, arg2Type a2) \
            : m_pObject(pObject)                                        \
            , m_fnMethod(method)                                        \
            , m_arg1(a1)                                                \
            , m_arg2(a2)                                                \
        {}                                                              \
        Typed##handleClass(const Typed##handleClass& src)               \
            : m_pObject(src.m_pObject)                                  \
            , m_fnMethod(src.m_fnMethod)                                \
            , m_arg1(src.m_arg1)                                        \
            , m_arg2(src.m_arg2)                                        \
        {}                                                              \
        ~Typed##handleClass()                                           \
        {                                                               \
        }                                                               \
    public:                                                             \
        inline void operator()(const asio::error_code&)                 \
        {                                                               \
            (m_pObject.get()->*m_fnMethod)(m_arg1, m_arg2);             \
        }                                                               \
        inline void operator()(arg1Type a1, arg2Type a2)                \
        {                                                               \
            (m_pObject.get()->*m_fnMethod)(a1, a2);                     \
        }                                                               \
        inline void operator()()                                        \
        {                                                               \
            (m_pObject.get()->*m_fnMethod)(m_arg1, m_arg2);             \
        }                                                               \
    private:                                                            \
        DEFINE_TYPED_PTR(T);                                            \
        ObjPtr m_pObject;                                               \
        Callback m_fnMethod;                                            \
        arg1Type m_arg1;                                                \
        arg2Type m_arg2;                                                \
    };                                                                  \
    typedef Typed##handleClass<thisClass> handleClass;                   \
    class handleClass##Binder                                           \
    {                                                                   \
    public:                                                             \
        template <typename T>                                           \
            static thisClass::Typed##handleClass<T>                     \
            bind(T* pObj, typename thisClass::Typed##handleClass<T>::Callback method, arg1Type a1, arg2Type a2) \
        {                                                               \
            return thisClass::Typed##handleClass<T>(pObj, method, a1, a2); \
        }                                                               \
    }

FX_NS_END

#endif //__FX_FUNCTIONBINDER_H
