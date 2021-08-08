//
// Created by HF on 2021/2/14.
//

#ifndef JSONPARSE_JSONPARSE_H
#define JSONPARSE_JSONPARSE_H

#include <iostream>
#include <string>
#include <memory>
#include <stack>
#include <assert.h>
#include <unordered_map>
#include "Document.h"

namespace json {
    /// json字符串解析
    class JsonParse {
    public:
        JsonParse();

        JsonParse(const std::string & value, int position);

        // 使用深度优先搜索树来进行解析
        Document Parse(const std::string & value = "", bool stop = false, char stopChar = ' ');

    private:
        // 获取pair的key
        std::string GetKey();

        // 去间隔符 :
        void Getinterval();

        // 是否空白字符
        bool is_white_space(const char & character);

        Document ParseDetail(const std::string & value, bool stop, char stopChar);

        Number ParseNumber();

        Bool ParseBool();

        Null ParseNull();

        Array<Document> ParseArray();

        std::string ParseString();

        int GetPosition() {
            return position_;
        }

    private:
        std::string context_;  // 原本内容
        int position_;  // 当前位置
        ErrorType error_type_;  // error type
    };

}  // json

#endif //JSONPARSE_JSONPARSE_H
