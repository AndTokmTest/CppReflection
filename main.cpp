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

#include <unistd.h>
#include <cerrno>
#include <filesystem>
#include <queue>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <linux/if.h>
#include <linux/ip.h>
#include <netinet/in.h>


namespace
{
    struct Integer
    {
        using value_type = int32_t;

        explicit Integer(value_type v): value { v } {
            std::println("Integer::Integer({})", value);
        }

        ~Integer() {
            std::println("Integer::~Integer({})", value);
        }

        Integer(const Integer& rhs) : value { rhs.value } {
            std::println("Integer(const Integer&)({})", value);
        }

        Integer& operator=(const Integer& rhs) {
            value = rhs.value;
            std::println("Integer& Integer::operator=(const Integer&)({})", value);
            return *this;
        }

        Integer(Integer&& rhs) noexcept : value { rhs.value }  {
            std::println("Integer(Integer&&({}) noexcept", value);
        }

        Integer& operator=(Integer&& rhs) noexcept {
            value = std::exchange(rhs.value, 0);
            std::println("Integer& Integer::operator=(Event&&)({}) noexcept", value);
            return *this;
        }

        [[nodiscard]]
        value_type getValue() const noexcept {
            return value;
        }

    private:

        value_type value { 0 };
    };


    struct Wrapper
    {
        std::unique_ptr<Integer> iPtr { nullptr };

        explicit Wrapper(int v): iPtr { std::make_unique<Integer>(v) } {
            std::println("Wrapper::Wrapper({})", iPtr->getValue());
        }

        ~Wrapper() {
            std::println("Wrapper::~Wrapper({})", iPtr->getValue());
        }

        Wrapper(const Wrapper&) {
            std::println("Wrapper(const Wrapper&)");
        }

        Wrapper& operator=(const Wrapper&) {
            std::println("Wrapper& Wrapper::operator=(const Wrapper&)");
            return *this;
        }

        Wrapper(Wrapper&&) noexcept {
            std::println("Wrapper(Wrapper&&) noexcept");
        }

        Wrapper& operator=(Wrapper&&) noexcept {
            std::println("Wrapper& Wrapper::operator=(Wrapper&&) noexcept");
            return *this;
        }
    };

}

namespace bit_utils
{
    constexpr inline uint8_t charBit { 8 };
    constexpr inline uint8_t maxSize { sizeof(u_int16_t) * charBit };

    [[nodiscard]]
    static constexpr bool isSet(const uint16_t mask, const uint16_t index) noexcept {
        return mask & (1u << index);
    }

    [[nodiscard]]
    static constexpr bool isNotSet(const uint16_t mask, const uint16_t index) noexcept {
        return not isSet(mask, index);
    }

    static constexpr void unsetBit(uint16_t& mask, const uint16_t bit) noexcept {
        mask &= ~(1 << bit);
    };

    static constexpr void setBit(uint16_t& mask, const uint16_t bit) noexcept {
        mask |= (1 << bit);
    }

    void printBits(const uint16_t mask)
    {
        std::cout << mask << "  ===>  ";
        for (int i = (sizeof(uint16_t) * charBit) - 1; i >= 0; --i)
            std::cout << (mask & (1u << i) ? '1' : '0');
        std::cout << std::endl;
    }
}


namespace demos
{
    struct ConstParam
    {
        int val { 0 };

        constexpr ConstParam() = default;
        constexpr explicit ConstParam(const int v): val { v } {
        }

        [[nodiscard]]
        constexpr int value() const noexcept {
            return val;
        }
    };


    template<ConstParam First = {}, ConstParam Second = {}>
    void sum(decltype(First), decltype(Second), int x)
    {
        if constexpr (First.value() == 1) {
            std::printf("FIRST IS ONE\n");
        }

        if constexpr (Second.value() == 2) {
            std::printf("SECOND IS TWO\n");
        }

        std::printf("SUM: %d\n", First.value() + Second.value() + x);
    }


    void test()
    {
        // demos::test();
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


    Integer v { 42 };


    return EXIT_SUCCESS;
}

