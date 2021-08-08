//
// Created by HF on 2021/3/7.
//

#ifndef JSONPARSE_META_H
#define JSONPARSE_META_H

#include <string>
#include <unordered_map>

namespace json {
    // Object
    template<typename T>
    class Object {
    public:
        T operator[](const std::string &name) {
            return context_[name];
        }

        void AddValue(std::string key, T doc) {
            context_.emplace(key, doc);
        }

        std::unordered_map<std::string, T> GetContext() {
            return context_;
        }

        std::string GetString() {
            std::string ret;
            ret.append("{ ");
            for ( auto iter = context_.begin();iter != context_.end(); ++iter) {
                ret.append("\"");
                ret.append(iter->first);
                ret.append("\" : ");
                ret.append(iter->second.GetString());
                ret.append(" ,");
            }
            ret.pop_back();
            ret.append(" }");
            return ret;
        }
    private:
        std::unordered_map<std::string, T> context_;
    };

    // Array
    template<typename T>
    class Array {
    public:
        T operator[](int i) {
            if (i < context_.size()) {
                return context_[i];
            }
            Document doc;
            doc.SetError(ErrorType::ArrayIndexError);
            return doc;
        }

        Array &operator=(const Array &array) {
            // todo
            this->context_ = array.context_;
            return *this;
        }

        T Value(int i) {
            if (i < context_.size()) {
                return context_[i];
            }
            T doc;
            doc.SetError(ErrorType::ArrayIndexError);
            return doc;
        }

        void AddValue(const T &doc) {
            context_.emplace_back(doc);
        }

        std::vector<T> GetContext()  {
            return context_;
        }

        std::string GetString() {
            std::string ret;
            ret.append("[ ");
            for ( auto & item : context_) {
                ret.append(item.GetString());
                ret.append(",");
            }
            ret.pop_back();
            ret.append(" ]");
            return ret;
        }

    private:
        // 直接使用对象的原因有二
        // 1.Document是一个小对象,频繁的生成和销毁不会浪费内存
        // todo:(关于频繁的创建 造成的内存碎片问题，使用tcmalloc 和 jemalloc替代std::allocator)
        // 2.可以更方便的存储混合类型的数组
        std::vector<T> context_;
    };

    // Number
    class Number : public std::string {
    public:
        Number() = default;

        Number(const char *str) : std::string(str) {}

        Number(std::string str) : std::string(str) {}

        int ToInt() {
            return std::stoi(this->c_str());
        }

        double ToDouble() {
            return std::stod(this->c_str());
        }

        float ToFloat() {
            return std::stof(this->c_str());
        };

        long ToLong() {
            return std::stol(this->c_str());
        }

        long long ToLongLong() {
            return std::stoll(this->c_str());
        }

        std::string Value() {
            return std::string(this->c_str());
        }

        void SetValue(const Number &number) {
            *this = number;
        }

        std::string GetString() {
            return *this;
        }
    };

    // Null
    class Null {
    public:
        void SetValue(bool j_null) {
            is_null_ = j_null;
        }

        bool Value() {
            return is_null_;
        }

        std::string GetString() {
            return "\"null\"";
        }
    private:
        bool is_null_;
    };

    // Bool
    class Bool {
    public:
        Bool() = default;

        explicit Bool(bool value) : value_(value) {}

        void SetValue(bool value) {
            value_ = value;
        }

        bool Value() {
            return value_;
        }

        Bool &operator=(bool value) {
            value_ = value;
            return *this;
        }

        std::string GetString() {
            if (value_) {
                return "\"true\"";
            }
            return "\"false\"";
        }
    private:
        bool value_;
    };
}
#endif //JSONPARSE_META_H
