//
//  JsonEncoder.hpp
//  JsonMapper
//
//  Created by 江派锋 on 16/4/26.
//  Copyright © 2016年 Gnefjp. All rights reserved.
//

#ifndef JsonEncoder_hpp
#define JsonEncoder_hpp

#include <vector>
#include <json/json.h>

//// protocol
//// make sure the custom object has member func: Json::Value toJson() const

namespace JsonEncoderDetail
{
    inline void setValueForKey(Json::Value &target, const Json::Value &value, const std::string &key);
    
    template<typename T>
    static Json::Value itemToJson(const T &item);
}

class JsonEncoder
{
public:
    JsonEncoder() {}
    JsonEncoder(const Json::Value &value) { _value = value; }
    
    const Json::Value& getValue() const { return _value; }
    
    template<typename T>
    JsonEncoder& Do(const T &fromValue, const std::string &key = "");
    
    template<typename T>
    JsonEncoder& Do(const std::vector<T> &fromValue, const std::string &key);
    
    
private:
    Json::Value _value;
};

template<typename T>
JsonEncoder& JsonEncoder::Do(const std::vector<T> &fromValue, const std::string &key)
{
    Json::Value value;
    for (auto &&obj : fromValue)
    {
        value.append(JsonEncoderDetail::itemToJson(obj));
    }
    JsonEncoderDetail::setValueForKey(_value, value, key);
    
    return *this;
}

template<typename T>
JsonEncoder& JsonEncoder::Do(const T &fromValue, const std::string &key)
{
    JsonEncoderDetail::setValueForKey(_value, JsonEncoderDetail::itemToJson(fromValue), key);
    return *this;
}

namespace JsonEncoderDetail
{
#define JSON_ENCODE_ITEM_FUNC(type) \
template<> \
__attribute__((unused)) Json::Value itemToJson<type>(const type &item) {\
return Json::Value(item); \
}
    JSON_ENCODE_ITEM_FUNC(Json::Int);
    JSON_ENCODE_ITEM_FUNC(Json::UInt);
#if defined(JSON_HAS_INT64)
    JSON_ENCODE_ITEM_FUNC(Json::Int64);
    JSON_ENCODE_ITEM_FUNC(Json::UInt64);
#endif
    JSON_ENCODE_ITEM_FUNC(double);
    JSON_ENCODE_ITEM_FUNC(Json::StaticString);
    JSON_ENCODE_ITEM_FUNC(std::string);
    JSON_ENCODE_ITEM_FUNC(bool);
    JSON_ENCODE_ITEM_FUNC(Json::Value);
    template<typename T>
    Json::Value itemToJson(const T &item)
    {
        return item.toJson();
    }
    
    void setValueForKey(Json::Value &target, const Json::Value &value, const std::string &key)
    {
        if (key.empty())
        {
            target = value;
            return;
        }
        
        target[key] = value;
    }
}
#endif /* JsonEncoder_hpp */
