//
//  CustomMapperTest.h
//  JsonMapper
//
//  Created by 江派锋 on 16/5/3.
//  Copyright © 2016年 Gnefjp. All rights reserved.
//

#ifndef CustomMapperTest_h
#define CustomMapperTest_h

#include "SimpleTest.h"

struct CustomMapperObject: public JsonMapper
{
    JSON_MEMBER(SimpleObject, normalObj);
    
    SimpleObject customObj;
    
    virtual void fromJson(const Json::Value &value) override
    {
        JsonMapper::fromJson(value);
        
        auto tmpStr = value["jsonData"].asString();
        if (tmpStr.empty())
        {
            return;
        }
        
        Json::Reader reader;
        Json::Value jsonData;
        if (reader.parse(tmpStr, jsonData))
        {
            customObj.fromJson(jsonData);
        }
    }
    
    virtual Json::Value toJson() const override
    {
        auto jsonData = customObj.toJson();
        if (jsonData.isNull())
        {
            return JsonMapper::toJson();
        }
        
        auto jsonValue = JsonMapper::toJson();
        auto writer = Json::FastWriter();
        jsonValue["jsonData"] = writer.write(jsonData);
        return jsonValue;
    }
};

static void doCustomMapperTest()
{
    printf("===================== begin CustomMapperTest\n");
    // to json
    Json::StyledWriter writer;
    
    CustomMapperObject toJson;
    auto defaultJson = toJson.toJson();
    printf("default value : %s\n", writer.write(defaultJson).c_str());
    
    initSimpleObject(toJson.normalObj);
    initSimpleObject(toJson.customObj);
    auto jsonValue = toJson.toJson();
    printf("json value : %s\n", writer.write(jsonValue).c_str());
    
    // from json
    CustomMapperObject fromJson;
    fromJson.fromJson(jsonValue);
    
    checkSimpleObject(toJson.normalObj, fromJson.normalObj);
    checkSimpleObject(toJson.customObj, fromJson.customObj);
    
    printf("===================== end CustomMapperTest\n");
}

#endif /* CustomMapper_h */
