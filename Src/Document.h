//
// Created by HF on 2021/3/7.
//

#ifndef JSONPARSE_DOCUMENT_H
#define JSONPARSE_DOCUMENT_H

#include <iostream>
#include <string>
#include <memory>
#include <stack>
#include <assert.h>
#include <unordered_map>
#include "Object.h"

namespace json {
    /// json对象
    class Document {
//        friend class Array; // 高效调用
    public:
        Document();

        std::string GetString();

        Type GetType() const {
            return type_;
        }

        void SetType(Type type);

        // 用于解析时分别解析错误
        ErrorType SetError(const ErrorType & type);

        ErrorType GetError();

        std::shared_ptr<JsonValue<Document>>  Value() {
            return value_;
        }

    private:

        void CreateValue(const Type & type);

        template<typename T, typename... Args>
        static T* create(Args&& ... args)
        {
            std::allocator<T> alloc;
            using AllocatorTraits = std::allocator_traits<std::allocator<T>>;

            auto deleter = [&](T * obj)
            {
                AllocatorTraits::deallocate(alloc, obj, 1);
            };
            std::unique_ptr<T, decltype(deleter)> obj(AllocatorTraits::allocate(alloc, 1), deleter);
            AllocatorTraits::construct(alloc, obj.get(), std::forward<Args>(args)...);
            assert(obj != nullptr);
            return obj.release();
        }

        template<typename T>
        static void deleter(T * obj)
        {
            std::allocator<T> alloc;
            using AllocatorTraits = std::allocator_traits<std::allocator<T>>;
            std::allocator_traits<decltype(alloc)>::destroy(alloc, obj);
            obj = nullptr;
        }

    private:
        Type type_; // json类型
        std::shared_ptr<JsonValue<Document>> value_; // json union
        ErrorType error_type_;  // error type
    };
}


#endif //JSONPARSE_DOCUMENT_H
