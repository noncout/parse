//
// Created by HF on 2021/2/14.
//

#include "JsonParse.h"
#include <assert.h>
#include "Document.h"
namespace json {
#define MARK 34 // '"'
#define SEPARAT 58 // ':'
#define COMMA 44 // ','
}
using namespace std;
using namespace json;

/////////////////
/// JsonParse ///
/////////////////

JsonParse::JsonParse()
        : context_(""),
          position_(0)
{
    error_type_ = ErrorType::NoError;
}

JsonParse::JsonParse(const string &value, int position)
        : context_(value),
          position_(position)
{
    error_type_ = ErrorType::NoError;
}

Document JsonParse::Parse( const std::string & value ,bool stop, char stopChar) {
    if (!value.empty()) {
        context_ = value;
    }
    return ParseDetail(context_, stop, stopChar);
}

// position_始终是待解析的第一个字符
Document JsonParse::ParseDetail(const string &value, bool stop, char stopChar) {
    Document doc;
    if (value.empty()) {
        doc.SetType(Type::Null);
        return doc;
    }
    stack<Type> stack;
    bool could_parse = false;
    while (position_ < context_.size()) {
        char current = context_[position_];
        if (!stack.empty() && stack.top() == Type::Object) {
            // 取key
            if (current == '"') {
                auto key = GetKey(); // "key"
                Getinterval(); // ":"
                JsonParse json(value, position_);
                auto obj_doc = json.Parse(value, true);
                position_ = json.GetPosition();
                doc.Value()->object_->AddValue(key, obj_doc);
                continue;
            }
        }

        // 样例，解析一个Object
        if (current == '{') {
            doc.SetType(Type::Object);
            stack.push(Type::Object);
            auto key = GetKey(); // "key"
            Getinterval(); // ":"
            JsonParse json(value, position_);
            auto obj_doc = json.Parse(value, true);
            position_ = json.GetPosition();
            doc.Value()->object_->AddValue(key, obj_doc);
            continue;
        }

        // 样例，解析一个Object
        if (current == '}') {
            auto type = stack.top();
            if (type != Type::Object) {
                error_type_ = ErrorType::ParseError;
                break;
            }
            stack.pop();
            if (stack.empty()) {
                break;
            }
        }

        // Array
        if (current == '[') {
            doc.SetType(Type::Array);
            ++position_;
            JsonParse json(value, position_);
            auto arr = json.ParseArray();
            position_ = json.GetPosition();
            *doc.Value()->array_ = arr;
            continue;
        }

        // String
        if (current == '"') {
            auto str = ParseString();
            doc.SetType(Type::String);
            doc.Value()->string_->append(str);
        }

        // Number
        if (current == '+' || current == '-' || isdigit(current)) {
            auto num = ParseNumber();
            doc.SetType(Type::Number);
            doc.Value()->number_->SetValue(num);
            continue;
        }

        // Bool
        if (current == 'f' || current == 't') {
            Bool j_bool = ParseBool();
            doc.SetType(Type::Bool);
            doc.Value()->bool_->SetValue(j_bool.Value());
            continue;
        }

        // Null
        if (current == 'N') {
            ParseNull();
            doc.SetType(Type::Null);
            doc.Value()->null_->SetValue(true);
            continue;
        }

        // ','
        if ( context_[position_] == COMMA) {
            if (stack.empty() && stop) {
                ++position_;
                break;
            }
        }

        if ( (context_[position_] == stopChar) && (stopChar != ' ') ) {
            break;
        }
        ++position_;
    }
    return doc;
}

Number JsonParse::ParseNumber() {
    Number number;
    while(position_ < context_.size()) {
        char current = context_[position_];
        if (current == '.' || current == '+'
            || current == '-' || isdigit(current)) {
            number.push_back(current);
            ++position_;
        } else {
            break;
        }
    }
    return number;
}

Bool JsonParse::ParseBool() {
    string j_true("true"), j_false("false"), value;
    while(position_ < context_.size()) {
        char current = context_[position_];
        if ( (j_false.find(current) != j_false.npos)
             || (j_true.find(current)) ) {
            value.push_back(current);
            ++position_;
        } else {
            --position_;
            break;
        }
    }
    Bool ret;
    if (value == j_true) {
        ret = true;
    } else if (value == j_false) {
        ret = false;
    } else {
        error_type_ = ErrorType::ParseError;
    }
    return ret;
}

Null JsonParse::ParseNull() {
    string j_null("null"), value;
    Null ret_null;
    while(position_ < context_.size()) {
        char current = context_[position_];
        if ( j_null.find(current) != j_null.npos) {
            value.push_back(current);
            ++position_;
        } else {
            --position_;
            break;
        }
    }
    if (value != j_null) {
        error_type_ = ErrorType::ParseError;
    }
    return ret_null;
}

//
Array<Document> JsonParse::ParseArray() {
    Array<Document> array;
    while(position_ < context_.size()) {
        auto doc = Parse(context_,true, ']');
        array.AddValue(doc);
        if (context_[position_] == COMMA) {
            ++position_;
        }
        if (context_[position_] == ']') {
            ++position_;
            break;
        }
    }
    return array;
}

// { "key" : "value" }
// todo:中文问题,使用编码格式转化函数进行解决
std::string JsonParse::GetKey() {
    std::string key;
    if (context_.empty()) {
        error_type_ = ErrorType::ParseError;
        return key;
    }
    bool could_append = false; // 是否开始取key
    while (position_ < context_.size()) {
        char current = context_[position_];
        if (int(current) == MARK) {
            if (!could_append) {
                // 开始取词
                could_append = true;
                ++position_;
                continue;
            } else {
                // 停止取词
                could_append = false;
                ++position_;
                break;
            }
        }
        if (could_append)
            key.push_back(current);
        ++position_;
    }
    return key;
}

// ':'
void JsonParse::Getinterval() {
    if (context_.empty()) {
        error_type_ = ErrorType::ParseError;
        return ;
    }

    while (position_ < context_.size()) {
        char current = context_[position_];
        if (int(current) == SEPARAT) {
            ++position_;
            break;
        }
        ++position_;
    }
}

// ' ' '\n' '\t' '\r'
bool JsonParse::is_white_space(const char & character) {
    if ( character == ' ' ||  character == '\n' ||
         character  == '\t' || character == '\r') {
        return true;
    }
    return false;
}

std::string JsonParse::ParseString() {
    std::string str;
    bool is_get = false;
    while(position_ < context_.size()) {
        char current = context_[position_];
        if ((int)current == MARK) {
            // 结束string解析
            if (is_get) {
                ++position_;
                break;
            } else {
                // 开始string解析
                is_get = true;
                ++position_;
                continue;
            }
        }
        if (is_get) {
            str.push_back(current);
        }
        ++position_;
    }
    return str;
}
