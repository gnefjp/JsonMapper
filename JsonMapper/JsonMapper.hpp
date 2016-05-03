//
//  JsonMapper.hpp
//  JsonMapper
//
//  Created by 江派锋 on 16/4/26.
//  Copyright © 2016年 Gnefjp. All rights reserved.
//

#ifndef JsonMapper_hpp
#define JsonMapper_hpp

#include <json/json.h>
#include <vector>
#include <functional>
#include "JsonEncoder.hpp"
#include "JsonDecoder.hpp"

template <typename T>
static T createDefaultValue();
#define JSON_DEFAULT_VALUE(type) \
template<> __attribute__((unused)) type createDefaultValue<type>() { return 0; }
JSON_DEFAULT_VALUE(uint32_t);
JSON_DEFAULT_VALUE(uint64_t);
JSON_DEFAULT_VALUE(int32_t);
JSON_DEFAULT_VALUE(int64_t);
JSON_DEFAULT_VALUE(float);
JSON_DEFAULT_VALUE(double);
JSON_DEFAULT_VALUE(bool);
template <typename T>
T createDefaultValue()
{
    return T();
}

class JsonMapper;
typedef std::function<void(JsonMapper *, JsonEncoder &)> DoJsonEncodeFunc;
typedef std::function<void(JsonMapper *, JsonDecoder &)> DoJsonDecodeFunc;

class JsonMapper
{
public:
    virtual void fromJson(const Json::Value &value);
    virtual Json::Value toJson() const;
    
private:
    template <typename T>
    friend class PropertyRegister;
    
    std::vector<DoJsonEncodeFunc> _encodeFuncs;
    std::vector<DoJsonDecodeFunc> _decodeFuncs;
};


template <class CT, typename T>
void JsonMemberDecode(JsonMapper *mapper, JsonDecoder &decoder, std::function<T&(CT *self)> getProperty, const std::string &key)
{
    decoder.Do(getProperty(static_cast<CT *>(mapper)), key);
}
template <class CT, typename T>
void JsonMemberEncode(JsonMapper *mapper, JsonEncoder &encoder, std::function<T&(CT *self)> getProperty, const std::string &key)
{
    encoder.Do(getProperty(static_cast<CT *>(mapper)), key);
}

template <class CT, typename T>
void JsonEnumDecode(JsonMapper *mapper, JsonDecoder &decoder, std::function<T&(CT *self)> getProperty, const std::string &key)
{
    int tmp = 0;
    decoder.Do(tmp, key);
    getProperty(static_cast<CT *>(mapper)) = static_cast<T>(tmp);
}
template <class CT, typename T>
void JsonEnumEncode(JsonMapper *mapper, JsonEncoder &encoder, std::function<T&(CT *self)> getProperty, const std::string &key)
{
    encoder.Do(static_cast<int>(getProperty(static_cast<CT *>(mapper))), key);
}

struct JsonEnumProperty{};
struct JsonMemberProperty{};
const static JsonEnumProperty JSON_ENUM_PROPERTY_TYPE = JsonEnumProperty();
const static JsonMemberProperty JSON_MEMBER_PROPERTY_TYPE = JsonMemberProperty();

template <typename T>
struct PropertyRegister
{
    template <class CT>
    PropertyRegister(JsonMapper *mapper, std::function<T&(CT *self)> getPropertyFunc, const char *key, JsonEnumProperty type) {
        using namespace std::placeholders;
        mapper->_decodeFuncs.push_back(std::bind(&JsonEnumDecode<CT, T>, _1, _2, getPropertyFunc, key));
        mapper->_encodeFuncs.push_back(std::bind(&JsonEnumEncode<CT, T>, _1, _2, getPropertyFunc, key));
    }
    
    template <class CT>
    PropertyRegister(JsonMapper *mapper, std::function<T&(CT *self)> getPropertyFunc, const char *key, JsonMemberProperty type) {
        using namespace std::placeholders;
        mapper->_decodeFuncs.push_back(std::bind(&JsonMemberDecode<CT, T>, _1, _2, getPropertyFunc, key));
        mapper->_encodeFuncs.push_back(std::bind(&JsonMemberEncode<CT, T>, _1, _2, getPropertyFunc, key));
    }
    
    T DefaultValue = createDefaultValue<T>();
    
    const T & getDefaultValue(const T &value) { return value; }
};
#define DefaultValue(x) getDefaultValue(x)


////////////////// choose macro
#define JSON_CHOOSE_MACRO(macro1, macro2, macro3, macro4, macro5, ...) macro5
#define JSON_CHOOSE_ERROR(param) static_assert(false, "wrong param count(2 <= params'count <= 3)")

////////////////// property
#define JSON_PROPERTY_GETTER(type, property) \
(std::function<type&(decltype(this))>)[](decltype(this) self) -> type& { return self->property; }

#define JSON_PROPERTY_HAS_KEY(type, property, key, pType)\
public:\
type property = PropertyRegister<type>(this, JSON_PROPERTY_GETTER(type, property), key, pType).DefaultValue

#define JSON_PROPERTY_NO_KEY(type, property, pType) JSON_PROPERTY_HAS_KEY(type, property, #property, pType)

#define JSON_PROPERTY_CHOOSE_MACRO(...) JSON_CHOOSE_MACRO(__VA_ARGS__, JSON_PROPERTY_HAS_KEY, JSON_PROPERTY_NO_KEY, JSON_CHOOSE_ERROR, )

/////////////////// enum-property & normal-property
#define JSON_MEMBER(...) JSON_PROPERTY_CHOOSE_MACRO(__VA_ARGS__, JSON_MEMBER_PROPERTY_TYPE)(__VA_ARGS__, JSON_MEMBER_PROPERTY_TYPE)
#define JSON_ENUM(...) JSON_PROPERTY_CHOOSE_MACRO(__VA_ARGS__, JSON_ENUM_PROPERTY_TYPE)(__VA_ARGS__, JSON_ENUM_PROPERTY_TYPE)

#endif /* JsonMapper_hpp */
