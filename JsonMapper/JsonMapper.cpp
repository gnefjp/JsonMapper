//
//  JsonMapper.cpp
//  JsonMapper
//
//  Created by 江派锋 on 16/4/26.
//  Copyright © 2016年 Gnefjp. All rights reserved.
//

#include "JsonMapper.hpp"

void JsonMapper::fromJson(const Json::Value &value)
{
    auto decoder = JsonDecoder(value);
    for (auto &func : _decodeFuncs)
    {
        func(this, decoder);
    }
}

Json::Value JsonMapper::toJson() const
{
    JsonEncoder encoder;
    for (auto &func : _encodeFuncs)
    {
        func(const_cast<JsonMapper *>(this), encoder);
    }
    return encoder.getValue();
}