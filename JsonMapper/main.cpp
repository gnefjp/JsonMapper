//
//  main.cpp
//  JsonMapper
//
//  Created by 江派锋 on 16/5/3.
//  Copyright © 2016年 Gnefjp. All rights reserved.
//

#include "SimpleTest.h"
#include "SubObjectTest.h"
#include "CustomMapperTest.h"
#include "InheritanceTest.h"

int main(int argc, const char * argv[])
{
    doSimpleTest();
    doCustomObjectTest();
    doCustomMapperTest();
    doInheritanceTest();
    return 0;
}
