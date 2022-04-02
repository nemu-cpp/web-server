/*
    Copyright (c) 2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/nemu-cpp/web-framework/blob/main/LICENSE.txt
*/

#include "SingleConnectionWebServerTests.hpp"
#include "Helpers/TestServerObserver.hpp"
#include "Helpers/TestWebRequestHandler.hpp"
#include "Nemu/WebFramework/SingleConnectionWebServer.hpp"
#include <boost/filesystem.hpp>
#include <Ishiko/HTTP.hpp>
#include <Ishiko/Networking.hpp>
#include <sstream>

using namespace boost::filesystem;
using namespace Ishiko;
using namespace Nemu;
using namespace std;

SingleConnectionWebServerTests::SingleConnectionWebServerTests(const TestNumber& number, const TestContext& context)
    : TestSequence(number, "SingleConnectionWebServer tests", context)
{
    append<HeapAllocationErrorsTest>("Constructor test 1", ConstructorTest1);
    append<HeapAllocationErrorsTest>("start test 1", StartTest1);
    append<FileComparisonTest>("Request test 1", RequestTest1);
    append<FileComparisonTest>("Request test 2", RequestTest2);
    append<FileComparisonTest>("Request test 3", RequestTest3);
}

void SingleConnectionWebServerTests::ConstructorTest1(Test& test)
{
    Error error;
    SingleConnectionWebServer server(IPv4Address::Localhost(), 0, error);

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_PASS();
}

void SingleConnectionWebServerTests::StartTest1(Test& test)
{
    Error error;
    SingleConnectionWebServer server(IPv4Address::Localhost(), 8585, error);

    ISHIKO_TEST_FAIL_IF(error);

    server.start();
    server.stop();
    server.join();

    ISHIKO_TEST_PASS();
}

void SingleConnectionWebServerTests::RequestTest1(FileComparisonTest& test)
{
    path outputPath(test.context().getTestOutputPath("SingleConnectionWebServerTests_RequestTest1.bin"));
    path referencePath(test.context().getReferenceDataPath("SingleConnectionWebServerTests_RequestTest1.bin"));

    std::shared_ptr<TestServerObserver> observer = std::make_shared<TestServerObserver>();
    Error error;
    SingleConnectionWebServer server(IPv4Address::Localhost(), TCPServerSocket::AnyPort, error);

    TestWebRequestHandler requestHandler;
    server.m_requestHandler = &requestHandler;

    server.start();

    std::ofstream responseFile(outputPath.string(), std::ios::out | std::ios::binary);
    HTTPClient::Get(IPv4Address::Localhost(), server.socket().port(), "/", responseFile, error);
    responseFile.close();

    server.stop();
    server.join();

    const std::vector<std::tuple<TestServerObserver::EEventType, const Nemu::Server*, std::string>>& events =
        observer->connectionEvents();

    /*
    * TODO
    ISHIKO_TEST_ABORT_IF_NEQ(events.size(), 2);
    ISHIKO_TEST_FAIL_IF_NEQ(std::get<0>(events[0]), TestServerObserver::eConnectionOpened);
    ISHIKO_TEST_FAIL_IF_NEQ(std::get<1>(events[0]), &server);
    ISHIKO_TEST_FAIL_IF_NEQ(std::get<2>(events[0]).substr(0, 10), "127.0.0.1:");
    ISHIKO_TEST_FAIL_IF_NEQ(std::get<0>(events[1]), TestServerObserver::eConnectionClosed);
    ISHIKO_TEST_FAIL_IF_NEQ(std::get<1>(events[1]), &server);
    ISHIKO_TEST_FAIL_IF_NEQ(std::get<2>(events[0]), std::get<2>(events[1]));
    */
    
    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(referencePath);

    ISHIKO_TEST_PASS();
}

void SingleConnectionWebServerTests::RequestTest2(FileComparisonTest& test)
{
    path outputPath(test.context().getTestOutputPath("SingleConnectionWebServerTests_RequestTest2.bin"));
    path referencePath(test.context().getReferenceDataPath("SingleConnectionWebServerTests_RequestTest2.bin"));

    std::shared_ptr<TestServerObserver> observer = std::make_shared<TestServerObserver>();
    Error error;
    SingleConnectionWebServer server(IPv4Address::Localhost(), TCPServerSocket::AnyPort, error);

    TestWebRequestHandler requestHandler;
    server.m_requestHandler = &requestHandler;

    server.start();

    // TODO: query for 2 different files, put output in 2 separate files and compare separately (needs improvements to
    // test framework
    std::ofstream responseFile(outputPath.string(), std::ios::out | std::ios::binary);
    HTTPClient::Get(IPv4Address::Localhost(), server.socket().port(), "/", responseFile, error);
    HTTPClient::Get(IPv4Address::Localhost(), server.socket().port(), "/", responseFile, error);
    responseFile.close();

    server.stop();

    server.join();

    const std::vector<std::tuple<TestServerObserver::EEventType, const Nemu::Server*, std::string>>& events =
        observer->connectionEvents();

    /*
    * TODO
    ISHIKO_TEST_ABORT_IF_NEQ(events.size(), 2);
    ISHIKO_TEST_FAIL_IF_NEQ(std::get<0>(events[0]), TestServerObserver::eConnectionOpened);
    ISHIKO_TEST_FAIL_IF_NEQ(std::get<1>(events[0]), &server);
    ISHIKO_TEST_FAIL_IF_NEQ(std::get<2>(events[0]).substr(0, 10), "127.0.0.1:");
    ISHIKO_TEST_FAIL_IF_NEQ(std::get<0>(events[1]), TestServerObserver::eConnectionClosed);
    ISHIKO_TEST_FAIL_IF_NEQ(std::get<1>(events[1]), &server);
    ISHIKO_TEST_FAIL_IF_NEQ(std::get<2>(events[0]), std::get<2>(events[1]));
    */

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(referencePath);

    ISHIKO_TEST_PASS();
}

void SingleConnectionWebServerTests::RequestTest3(FileComparisonTest& test)
{
    path outputPath(test.context().getTestOutputPath("SingleConnectionWebServerTests_RequestTest3.bin"));
    path referencePath(test.context().getReferenceDataPath("SingleConnectionWebServerTests_RequestTest3.bin"));

    std::shared_ptr<TestServerObserver> observer = std::make_shared<TestServerObserver>();
    Error error;
    SingleConnectionWebServer server(IPv4Address::Localhost(), TCPServerSocket::AnyPort, error);
    Port port = server.socket().port();

    TestWebRequestHandler requestHandler;
    server.m_requestHandler = &requestHandler;
 
    // TODO: we should try to connect before the socket is opened but at the moment with my blocking HTTPClient I can't
    // do that

    server.start();

    std::ofstream responseFile(outputPath.string(), std::ios::out | std::ios::binary);
    HTTPClient::Get(IPv4Address::Localhost(), port, "/", responseFile, error);
    responseFile.close();

    server.stop();

    server.join();

    // At this stage the server is stopped but not destroyed. Verify that attempts to connect fail.


    const std::vector<std::tuple<TestServerObserver::EEventType, const Nemu::Server*, std::string>>& events =
        observer->connectionEvents();

    stringstream output;
    HTTPClient::Get(IPv4Address::Localhost(), port, "/", output, error);

    ISHIKO_TEST_FAIL_IF_NOT(error);
    // TODO: check error type

    /*
    * TODO
    ISHIKO_TEST_ABORT_IF_NEQ(events.size(), 2);
    ISHIKO_TEST_FAIL_IF_NEQ(std::get<0>(events[0]), TestServerObserver::eConnectionOpened);
    ISHIKO_TEST_FAIL_IF_NEQ(std::get<1>(events[0]), &server);
    ISHIKO_TEST_FAIL_IF_NEQ(std::get<2>(events[0]).substr(0, 10), "127.0.0.1:");
    ISHIKO_TEST_FAIL_IF_NEQ(std::get<0>(events[1]), TestServerObserver::eConnectionClosed);
    ISHIKO_TEST_FAIL_IF_NEQ(std::get<1>(events[1]), &server);
    ISHIKO_TEST_FAIL_IF_NEQ(std::get<2>(events[0]), std::get<2>(events[1]));
    */

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(referencePath);

    ISHIKO_TEST_PASS();
}