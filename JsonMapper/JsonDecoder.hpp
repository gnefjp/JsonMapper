//
//  JsonDecoder.hpp
//  JsonMapper
//
//  Created by 江派锋 on 16/4/26.
//  Copyright © 2016年 Gnefjp. All rights reserved.
//

#ifndef JsonDecoder_hpp
#define JsonDecoder_hpp

#include <assert.h>
#include <json/json.h>

//// protocol
//// make sure the custom object has member func: fromJson(const Json::Value &jsonValue)

namespace JsonDecodeDetail
{
    inline const Json::Value & getValueFromKey(const Json::Value &jsonValue, const std::string &key);
    
    template<typename T>
    static void item(T &toValue, const Json::Value &jsonValue, const std::string &key);
    
    template<typename T>
    void list(T &toList, const Json::Value &jsonValue, const std::string &key);
}

class JsonDecoder
{
public:
    JsonDecoder(const Json::Value &jsonValue): _value(jsonValue){}
    
    template<typename T>
    const JsonDecoder & Do(T &toValue, const std::string &key = "") const;
    
    template<typename T>
    const JsonDecoder & Do(std::vector<T> &toList, const std::string &key = "") const;
    
private:
    const Json::Value &_value;
};

template<typename T>
const JsonDecoder & JsonDecoder::Do(T &toValue, const std::string &key) const
{
    JsonDecodeDetail::item(toValue, _value, key);
    return *this;
}

template<typename T>
const JsonDecoder & JsonDecoder::Do(std::vector<T> &toList, const std::string &key) const
{
    JsonDecodeDetail::list(toList, _value, key);
    return *this;
}

namespace JsonDecodeDetail
{
    template<typename T>
    void list(T &toList, const Json::Value &jsonValue, const std::string &key)
    {
        const Json::Value &targetValue = getValueFromKey(jsonValue, key);
        if (targetValue.type() != Json::ValueType::arrayValue)
        {
            toList.clear();
            return;
        }
        auto size = targetValue.size();
        toList.clear();
        toList.resize(size);
        for (auto i = 0; i < size; ++i)
        {
            item(toList[i], targetValue[i], "");
        }
    }
    
#define JSON_DECODE_ITEM_FUNC(type, func) \
template<> \
__attribute__((unused))  void item<type>(type &toValue, const Json::Value &jsonValue, const std::string &key) { \
toValue = getValueFromKey(jsonValue, key).func(); \
}
    
    JSON_DECODE_ITEM_FUNC(std::string, asString);
    JSON_DECODE_ITEM_FUNC(Json::Int, asInt);
    JSON_DECODE_ITEM_FUNC(Json::UInt, asUInt);
    JSON_DECODE_ITEM_FUNC(Json::LargestInt, asLargestInt);
    JSON_DECODE_ITEM_FUNC(Json::LargestUInt, asLargestUInt);
    JSON_DECODE_ITEM_FUNC(float, asFloat);
    JSON_DECODE_ITEM_FUNC(double, asDouble);
    JSON_DECODE_ITEM_FUNC(bool, asBool);
    
    template<>
    __attribute__((unused)) void item<Json::Value>(Json::Value &toValue, const Json::Value &jsonValue, const std::string &key)
    {
        toValue = getValueFromKey(jsonValue, key);
    }
    
    template<typename T>
    void item(T &toValue, const Json::Value &jsonValue, const std::string &key)
    {
        toValue.fromJson(getValueFromKey(jsonValue, key));
    }
    
    const Json::Value & getValueFromKey(const Json::Value &jsonValue, const std::string &key)
    {
        if (key.empty() || jsonValue.type() != Json::ValueType::objectValue)
        {
            return jsonValue;
        }
        
        return jsonValue[key];
    }
}

#endif /* JsonDecoder_hpp */
