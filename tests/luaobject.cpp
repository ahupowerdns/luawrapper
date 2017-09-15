#include <LuaContext.hpp>
#include <gtest/gtest.h>

TEST(LuaObject, RoundTripPlusRefCounting) {
    LuaContext context;
    
    LuaContext::LuaObject v1, v2;

    context.executeCode("function regsize() local count=0 for k,v in pairs(debug.getregistry()) do count=count+1 end return count end");
    context.executeCode("function f1() return {foo='bar'} end");
    context.executeCode("function f2(o) return type(o) end");

    auto regsize = context.readVariable<std::function<int()>>("regsize");

    auto f1 = context.readVariable<std::function<LuaContext::LuaObject()>>("f1");
    auto f2 = context.readVariable<std::function<std::string(LuaContext::LuaObject)>>("f2");

    int regsize0 = regsize();

    v1 = f1();
    EXPECT_EQ("table", f2(v1));

    EXPECT_EQ(regsize0 + 1, regsize());

    v2 = f1();
    EXPECT_EQ(regsize0 + 2, regsize());

    v1 = v2;
    EXPECT_EQ(regsize0 + 1, regsize());
}
