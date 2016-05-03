//
//  SubObjectTest.h
//  JsonMapper
//
//  Created by 江派锋 on 16/5/3.
//  Copyright © 2016年 Gnefjp. All rights reserved.
//

#ifndef SubObjectTest_h
#define SubObjectTest_h

#include "SimpleTest.h"

struct SubObject: public JsonMapper
{
    JSON_MEMBER(SimpleObject, object);
    JSON_MEMBER(std::vector<SimpleObject>, array);
};

static void doCustomObjectTest()
{
    printf("===================== begin CustomObjectTest\n");
    // to json
    Json::StyledWriter writer;
    
    SubObject toJson;
    auto defaultJson = toJson.toJson();
    printf("default value : %s\n", writer.write(defaultJson).c_str());
    
    initSimpleObject(toJson.object);
    toJson.array.resize(3);
    for (int i = 0; i < 3; ++i)
    {
        initSimpleObject(toJson.array[i]);
    }
    auto jsonValue = toJson.toJson();
    printf("json value : %s\n", writer.write(jsonValue).c_str());
    
    // from json
    SubObject fromJson;
    fromJson.fromJson(jsonValue);
    
    checkSimpleObject(toJson.object, fromJson.object);
    assert(toJson.array.size() == fromJson.array.size());
    for (int i = 0; i < toJson.array.size(); ++i)
    {
        checkSimpleObject(toJson.array[i], fromJson.array[i]);
    }
    
    printf("===================== end CustomObjectTest\n");
}

#endif /* CustomObjectTest_h */
