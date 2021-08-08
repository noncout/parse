//
// Created by HF on 2021/2/14.
//

#ifndef JSONPARSE_OBJECT_H
#define JSONPARSE_OBJECT_H
#include <string>
#include "Meta.h"

namespace json {
    enum class Type {
        Object = 0,
        Array,
        String,
        Bool,
        Number,
        Null,
        NotDefine,
    };

    enum class ErrorType {
        NoError = 0,
        /// 字符串解析错误
        ParseError,
        ArrayIndexError,  // 数组位置参数不正确
        // Doc错误
    };

    // json字符串类型
    template <typename T>
    union JsonValue {
        Object<T> * object_;
        Array<T> * array_;
        std::string * string_;
        Number * number_;
        Null * null_;
        Bool * bool_;
    };

}  // json

#endif //JSONPARSE_OBJECT_H
