// Third-Party
#include <boost/version.hpp>

#pragma warning(disable: 4267)
#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>
#pragma warning(default: 4267)

#include <sol.hpp>

#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>

// Standard
#include <memory>
#include <functional>
#include <cstring>
#include <string>
#include <map>
#include <vector>
#include <deque>
#include <limits>
#include <cassert>
