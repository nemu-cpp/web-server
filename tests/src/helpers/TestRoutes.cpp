/*
    Copyright (c) 2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/nemu-cpp/ishiko-connection-engine/blob/main/LICENSE.txt
*/

#include "TestRoutes.hpp"
#include <Ishiko/Time.hpp>

using namespace Ishiko;
using namespace Nemu;
using namespace std;

TestRoutes::TestRoutes()
{
    setDefaultRoute(
        Route("",
            make_shared<FunctionWebRequestHandler>(
                [](const WebRequest& request, WebResponseBuilder& response, void* handlerData, Logger& logger)
                {
                    response.setDateHeader(UTCTime(Date(2022, 3, 24), TimeOfDay(1, 15, 0)));
                    response.setStatus(404);
                })));
}