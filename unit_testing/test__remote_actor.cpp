#include <string>
#include <sstream>
#include <iostream>

#include "test.hpp"
#include "ping_pong.hpp"
#include "cppa/cppa.hpp"
#include "cppa/exception.hpp"
#include "cppa/detail/thread.hpp"

using std::cout;
using std::cerr;
using std::endl;

using namespace cppa;

namespace {

void client_part(std::vector<string_pair> const& args)
{
    auto i = std::find_if(args.begin(), args.end(),
                          [](string_pair const& p) { return p.first == "port"; });
    if (i == args.end())
    {
        throw std::runtime_error("no port specified");
    }
    std::istringstream iss(i->second);
    std::uint16_t port;
    iss >> port;
    auto ping_actor = remote_actor("localhost", port);
    spawn(pong, ping_actor);
    await_all_others_done();
}

} // namespace <anonymous>

size_t test__remote_actor(char const* app_path, bool is_client,
                          std::vector<string_pair> const& args)
{
    if (is_client)
    {
        client_part(args);
        return 0;
    }
    CPPA_TEST(test__remote_actor);
    auto ping_actor = spawn(ping);
    std::uint16_t port = 4242;
    bool success = false;
    do
    {
        try
        {
            publish(ping_actor, port);
            success = true;
        }
        catch (bind_failure&)
        {
            // try next port
            ++port;
        }
    }
    while (!success);
    std::ostringstream oss;
    oss << app_path << " run=remote_actor port=" << port;// << " &>/dev/null";
    // execute client_part() in a separate process,
    // connected via localhost socket
    detail::thread child([&oss]()
    {
        std::string cmdstr = oss.str();
        if (system(cmdstr.c_str()) != 0)
        {
            cerr << "FATAL: command \"" << cmdstr << "\" failed!" << endl;
            abort();
        }
    });
    await_all_others_done();
    CPPA_CHECK_EQUAL(pongs(), 5);
    // wait until separate process (in sep. thread) finished execution
    child.join();
    return CPPA_TEST_RESULT;
}
