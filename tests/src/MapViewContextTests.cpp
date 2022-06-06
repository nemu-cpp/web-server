/*
    Copyright (c) 2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/nemu-cpp/web-framework/blob/main/LICENSE.txt
*/

#include "MapViewContextTests.hpp"
#include "Nemu/WebFramework/MapViewContext.hpp"
#include <string>
#include <vector>

using namespace Ishiko;
using namespace Nemu;

MapViewContextTests::MapViewContextTests(const TestNumber& number, const TestContext& context)
    : TestSequence(number, "MapViewContext tests", context)
{
    append<HeapAllocationErrorsTest>("Constructor test 1", ConstructorTest1);
    append<HeapAllocationErrorsTest>("getValue test 1", GetValueTest1);
    append<HeapAllocationErrorsTest>("getValue test 2", GetValueTest2);
}

void MapViewContextTests::ConstructorTest1(Test& test)
{
    MapViewContext viewContext;

    ISHIKO_TEST_PASS();
}

void MapViewContextTests::GetValueTest1(Ishiko::Test& test)
{
    MapViewContext viewContext;
    viewContext.map()["var1"] = "value1";

    Ishiko::Error error;
    ViewContext::Value result = viewContext.getValue("var1", error);

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_ABORT_IF_NEQ(result.type(), ViewContext::Value::Type::string);
    ISHIKO_TEST_FAIL_IF_NEQ(result.asString(), "value1");
    ISHIKO_TEST_PASS();
}

void MapViewContextTests::GetValueTest2(Ishiko::Test& test)
{
    MapViewContext viewContext;
    viewContext.map()["var1"] = std::vector<std::string>({ "value1", "value2" });

    Ishiko::Error error;
    ViewContext::Value result = viewContext.getValue("var1", error);

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_ABORT_IF_NEQ(result.type(), ViewContext::Value::Type::stringArray);
    ISHIKO_TEST_FAIL_IF_NEQ(result.asStringArray(), std::vector<std::string>({ "value1", "value2" }));
    ISHIKO_TEST_PASS();
}
