#include <iostream>
#include "JsonParse.h"

using namespace std;
using namespace json;
int main() {
    std::string json = "{\"project\":[1,2,3],\"stars\":\"hello\"}";
    JsonParse parse;
    auto j = parse.Parse(json);
    std::cout << j.GetString();
    return 0;
}
