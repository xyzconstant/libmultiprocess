// Copyright (c) The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef MP_PROXY_TYPE_RUNTIME_H
#define MP_PROXY_TYPE_RUNTIME_H

#include <mp/util.h>

namespace mp {
template <>
struct ProxyServer<Runtime> final : public virtual Runtime::Server
{
public:
    ProxyServer(Connection& connection);
    kj::Promise<void> makeThread(MakeThreadContext context) override;
    kj::Promise<void> makePool(MakePoolContext context) override;
    Connection& m_connection;
};

template <typename Output>
void CustomBuildField(TypeList<>,
    Priority<1>,
    InvokeContext& invoke_context,
    Output&& output,
    typename std::enable_if<std::is_same<decltype(output.get()), Runtime::Client>::value>::type* enable = nullptr)
{
    output.set(kj::heap<ProxyServer<Runtime>>(invoke_context.connection));
}

template <typename Input>
decltype(auto) CustomReadField(TypeList<>,
    Priority<1>,
    InvokeContext& invoke_context,
    Input&& input,
    typename std::enable_if<std::is_same<decltype(input.get()), Runtime::Client>::value>::type* enable = nullptr)
{
    invoke_context.connection.m_runtime = input.get();
}
} // namespace mp

#endif // MP_PROXY_TYPE_RUNTIME_H
