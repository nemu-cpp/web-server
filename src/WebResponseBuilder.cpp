/*
    Copyright (c) 2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/nemu-cpp/web-framework/blob/main/LICENSE.txt
*/

#include "WebResponseBuilder.hpp"

using namespace Nemu;

WebResponseBuilder::WebResponseBuilder()
    : m_response(Ishiko::HTTPStatusCode::internalServerError), m_views(nullptr)
{
    // TODO: this doesn't work as the user would be left with specifying all the right headers. Do I assume some default?
}

void WebResponseBuilder::setStatus(unsigned int status)
{
    m_response.setStatusCode(status);
}
    
void WebResponseBuilder::setDateHeader(const Ishiko::UTCTime& time)
{
    m_response.setDateHeader(time);
}

std::string& WebResponseBuilder::body()
{
    return m_body;
}

std::string WebResponseBuilder::toString() const
{
    // TODO: wasteful
    Ishiko::HTTPResponse response(m_response);
    response.setContentLengthHeader(m_body.size());
    response.setBody(m_body);
    return response.toString();
}

void WebResponseBuilder::view(const std::string& view, ViewContext& context)
{
    // TODO: correct error handling and tests
    m_response.setStatusCode(Ishiko::HTTPStatusCode::ok);
    body() = m_views->defaultProfile().render(view, context, nullptr);
}

void WebResponseBuilder::view(const std::string& view, ViewContext& context, const std::string& layout)
{
    // TODO: correct error handling and tests
    m_response.setStatusCode(Ishiko::HTTPStatusCode::ok);
    body() = m_views->defaultProfile().render(view, context, &layout);
}

void WebResponseBuilder::view(const std::string& profile, const std::string& view, ViewContext& context)
{
    // TODO: correct error handling and tests
    m_response.setStatusCode(Ishiko::HTTPStatusCode::ok);
    body() = m_views->profile(profile).render(view, context, nullptr);
}

void WebResponseBuilder::view(const std::string& profile, const std::string& view, ViewContext& context,
    const std::string& layout)
{
    // TODO: correct error handling and tests
    m_response.setStatusCode(Ishiko::HTTPStatusCode::ok);
    body() = m_views->profile(profile).render(view, context, &layout);
}

void WebResponseBuilder::redirect()
{
    // TODO
}

void WebResponseBuilder::state()
{
    // TODO
}
