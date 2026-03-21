/**============================================================================
Name        : Tests.cpp
Created on  : 15.09.2021
Author      : Tokmakov Andrei
Version     : 1.0
Copyright   : Your copyright notice
Description : Tests C++ project
============================================================================**/

#include <iostream>
#include <syncstream>
#include <iomanip>
#include <format>
#include <print>

#include <bitset>

#include <fstream>
#include <thread>
#include <functional>
#include <memory>
#include <concepts>
#include <utility>
#include <string_view>
#include <string>
#include <cassert>
#include <random>
#include <tuple>
#include <bit>
#include <utility>
#include <condition_variable>
#include <source_location>

#include <algorithm>
#include <ranges>
#include <set>
#include <map>
#include <vector>
#include <deque>
#include <unordered_set>
#include <unordered_map>

#include <coroutine>
#include <meta>

namespace reflection::types
{
    struct User
    {
        std::string name;
        int age;
    };

    constexpr int max_connections = 100;

    [[maybe_unused]]
    void say_hello() { }


    [[maybe_unused]]
    void reflectTypes_Simple()
    {
        // Reflect the struct type 'User'
         [[maybe_unused]]
        constexpr std::meta::info user_reflection = ^^User;

        // Reflect the global variable 'max_connections'
         [[maybe_unused]]
        constexpr std::meta::info connections_reflection = ^^max_connections;

        // Reflect the free function 'say_hello'
         [[maybe_unused]]
        constexpr std::meta::info hello_reflection = ^^say_hello;

        // Reflect the data member 'age' inside struct 'User'
         [[maybe_unused]]
        constexpr std::meta::info user_age_reflection = ^^User::age;
    }

    void useReflectedData()
    {
        constexpr std::meta::info int_reflection = ^^int;
        constexpr std::meta::info double_reflection = ^^double;
        constexpr std::meta::info string_reflection = ^^std::string;

        typename [: int_reflection :] number = 100;
        typename [: double_reflection :] pi = 3.14;
        typename [: string_reflection :] message = "Hello Reflection";

        std::cout << "Number: " << number << std::endl;
        std::cout << "Pi: " << pi << std::endl;
        std::cout << "Message: " << message << std::endl;

        // Number: 100
        // Pi: 3.14
        // Message: Hello Reflection
    }
}


int main(const int argc,
         char** argv,
         char** environment)
{
    const std::vector<std::string_view> args(argv + 1, argv + argc);
    const std::map<std::string_view, std::string_view> env = [&] {
        std::map<std::string_view, std::string_view> envs;
        for (int i = 0; environment && environment[i]; ++i) {
            const std::string_view envVar = environment[i];
            if (const size_t pos = envVar.find('='); std::string::npos != pos) {
                envs.emplace(envVar.substr(0, pos), envVar.substr(pos + 1));
            }
        }
        return envs;
    }();

    using namespace reflection;


    types::reflectTypes_Simple();
    types::useReflectedData();

    return EXIT_SUCCESS;
}

