//
// Created by HF on 2021/3/7.
//

#include "Document.h"

using namespace std;
using namespace json;

Document::Document()
        : type_(Type::Null),
          error_type_(ErrorType::NoError)
{

}

void Document::SetType(Type type) {
    type_ = type;
    CreateValue(type); // value_
}

void Document::CreateValue(const Type &type) {
    value_ = std::make_shared<JsonValue<Document>>();
    switch (type) {
        case Type::Object :
            value_->object_ = create<Object<Document>>();
            break;
        case Type::Array :
            value_->array_ = create<Array<Document>>();
            break;
        case Type::String :
            value_->string_ = create<std::string>();
            break;
        case Type::Bool :
            value_->bool_ = create<Bool>();
            break;
        case Type::Number :
            value_->number_ = create<Number>();
            break;
        case Type::Null :
        case Type::NotDefine :
        default:
            break;
    }
}

ErrorType Document::SetError(const ErrorType &type) {
    ErrorType old_type = error_type_;
    error_type_ = type;
    return old_type;
}

ErrorType Document::GetError() {
    return error_type_;
}

std::string Document::GetString() {
    std::string ret;
    switch (type_) {
        case Type::Object:
            ret = value_->object_->GetString();
            break;
        case Type::Array:
            ret = value_->array_->GetString();
            break;
        case Type::String:
            ret = "\"" + *value_->string_ + "\"";
            break;
        case Type::Bool:
            ret = value_->bool_->GetString();
            break;
        case Type::Number:
            ret = value_->number_->GetString();
            break;
        case Type::Null:
            ret = value_->null_->GetString();
            break;
        default:
            break;
    }
    return ret;
}
