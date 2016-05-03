//
//  SimpleTest.h
//  JsonMapper
//
//  Created by 江派锋 on 16/5/3.
//  Copyright © 2016年 Gnefjp. All rights reserved.
//

#ifndef SimpleTest_h
#define SimpleTest_h

#include "JsonMapper.hpp"

enum class SimpleEnum
{
    SimpleEnum0 = 0,
    SimpleEnum1 = 1,
    SimpleEnum2 = 2,
};

struct SimpleObject: public JsonMapper
{
    JSON_MEMBER(bool, boolValue);
    JSON_MEMBER(int, intValue);
    JSON_MEMBER(double, doubleValue);
    JSON_MEMBER(std::string, stringValue);
    JSON_ENUM(SimpleEnum, enumValue);
    JSON_MEMBER(std::vector<int>, intArrayValue);
//    JSON_MEMBER(std::vector<SimpleEnum>, enumArrayValue); not suppor enum array
    
    JSON_ENUM(SimpleEnum, specialDefaultValue)(SimpleEnum::SimpleEnum2);
    JSON_MEMBER(int, specialKeyValue, "diffJsonKey");
};

static void initSimpleObject(SimpleObject &obj)
{
    obj.boolValue = true;
    obj.intValue = 100;
    obj.doubleValue = 1.11;
    obj.stringValue = "hello world";
    obj.enumValue = SimpleEnum::SimpleEnum1;
    obj.intArrayValue = { 1, 2, 3, 4, 5, 6 };
    obj.specialDefaultValue = SimpleEnum::SimpleEnum0;
    obj.specialKeyValue = 999;
}

static void checkSimpleObject(const SimpleObject &obj1, const SimpleObject &obj2)
{
    assert(obj1.boolValue == obj2.boolValue);
    assert(obj1.intValue == obj2.intValue);
    assert(obj1.doubleValue == obj2.doubleValue);
    assert(obj1.stringValue == obj2.stringValue);
    assert(obj1.enumValue == obj2.enumValue);
    assert(obj1.intArrayValue.size() == obj2.intArrayValue.size());
    for (int i = 0; i < obj1.intArrayValue.size(); ++i)
    {
        assert(obj1.intArrayValue[i] == obj2.intArrayValue[i]);
    }
    assert(obj1.specialDefaultValue == obj2.specialDefaultValue);
    assert(obj1.specialKeyValue == obj2.specialKeyValue);
}

static void doSimpleTest()
{
    printf("===================== begin SimpleTest\n");
    // to json
    Json::StyledWriter writer;
    
    SimpleObject toJson;
    auto defaultJson = toJson.toJson();
    printf("default value : %s\n", writer.write(defaultJson).c_str());
    
    initSimpleObject(toJson);
    auto jsonValue = toJson.toJson();
    printf("json value : %s\n", writer.write(jsonValue).c_str());
    
    // from json
    SimpleObject fromJson;
    fromJson.fromJson(jsonValue);
    
    checkSimpleObject(toJson, fromJson);
    printf("===================== end SimpleTest\n");
}

#endif /* SimpleTest_h */
