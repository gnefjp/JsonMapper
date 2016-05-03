//
//  InheritanceTest.h
//  JsonMapper
//
//  Created by 江派锋 on 16/5/3.
//  Copyright © 2016年 Gnefjp. All rights reserved.
//

#ifndef InheritanceTest_h
#define InheritanceTest_h

#include "SimpleTest.h"

// first inheritance
struct InheritanceObj1 : public SimpleObject
{
    JSON_MEMBER(std::string, testMember);
};

struct NormalSuperClass
{
    std::string someMember = "NormalSuperClass.someMember";
};

// second inheritance 指针偏移问题
struct InheritanceObj2 : public NormalSuperClass, public SimpleObject
{
    JSON_MEMBER(std::string, testMember);
};

template <class T>
static void testForObj()
{
    // to json
    Json::StyledWriter writer;
    
    T toJson;
    auto defaultJson = toJson.toJson();
    printf("default value : %s\n", writer.write(defaultJson).c_str());
    
    initSimpleObject(toJson);
    toJson.testMember = "testMember";
    auto jsonValue = toJson.toJson();
    printf("json value : %s\n", writer.write(jsonValue).c_str());
    
    // from json
    T fromJson;
    fromJson.fromJson(jsonValue);
    
    checkSimpleObject(toJson, fromJson);
    assert(toJson.testMember == fromJson.testMember);
}

static void doInheritanceTest()
{
    printf("===================== begin InheritanceTest\n");
    testForObj<InheritanceObj1>();
    testForObj<InheritanceObj1>();
    printf("===================== end InheritanceTest\n");
}

#endif /* MultipleInheritanceTest_h */
