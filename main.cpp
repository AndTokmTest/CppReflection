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

namespace reflection
{
    struct User
    {
        std::string name;
        int age { 0 };
    };

    enum class RGB {
        Red,
        Blue,
        Green
    };

    enum class Color {
        Red,
        Green,
        Blue,
        Yellow,
        Purple
    };

    enum class Animal {
        Cat,
        Dog,
        Horse,
        Rabbit,
        Snail
    };
}

namespace reflection::types
{
    constexpr int max_connections = 100;

    void say_hello() { }


    void basics()
    {
        constexpr auto r = ^^int;

        typename[:r:] i = 42;       // Same as: int x = 42;
        static_assert(std::is_same_v<decltype(i), int>);

        typename[:^^char:] c = '*';  // Same as: char c = '*';
        static_assert(std::is_same_v<decltype(c), char>);
    }

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

        std::cout << "Number: " << number << " of type " << typeid(number).name() << std::endl;
        std::cout << "Pi: " << pi  << " of type " << typeid(pi).name() << std::endl;
        std::cout << "Message: " << message  << " of type " << typeid(message).name() << std::endl;

        // Number: 100
        // Pi: 3.14
        // Message: Hello Reflection
    }


    template <typename T>
    void deduce_Type_of_Vector()
    {
        constexpr auto vec = ^^std::vector<T>;
        typename [: vec :] x = { T(1), T(2), T(3)};
        for (const auto& elem : x) {
            std::cout << elem << " ";
        }
        std::cout << std::endl;

        // Output: 1 2 3
    }

    template <typename T>
    struct MyVec
    {
        std::array<T, 3> data;
    };

    template <typename Key, typename Value>
    struct MyMap
    {
        Key key;
        Value value;
    };

    void Create_New_Type()
    {
        // Reflect the individual types we'll need
        constexpr std::meta::info key_info     = ^^int;       // Get meta info for 'int'
        constexpr std::meta::info vec_template = ^^MyVec;     // Get meta info for the template 'MyVec'
        constexpr std::meta::info value_arg    = ^^double;    // Get meta info for 'double'

        // Use reflection to create the type MyVec<double>
        // This "fills in" the template MyVec<T> with T = double
        constexpr std::meta::info vec_info = std::meta::substitute(vec_template, {value_arg});

        // Use reflection to create the type MyMap<int, MyVec<double>>
        // This creates a reflected instantiation of MyMap with the previous types
        constexpr std::meta::info map_info = std::meta::substitute(^^MyMap, {key_info, vec_info});

        // Splice the reflected type into real C++ code using [: :]
        // This gives us a real usable type: MyMap<int, MyVec<double>>
        typename [:map_info:] obj = {
            7,                              // obj.key = 7
            { {1.1, 2.2, 3.3} }             // obj.value = MyVec<double> with 3 doubles
        };

        // Use the object like any normal type
        std::cout << "Key: " << obj.key << std::endl;
        std::cout << "Values:\n";
        for (double v : obj.value.data)  // Loop through the MyVec's data array
            std::cout << v << '\n';

        /*
        Key: 7
        Values:
        1.1
        2.2
        3.3
        */
    }
}


namespace reflection::get_data_member_0
{
    void retrieve_Data_Members_Names()
    {
        // Reflect the type User
        constexpr std::meta::info user_reflection = ^^User;

        // Retrieve the name of the reflected type
        std::cout << "Type: " << std::meta::identifier_of(user_reflection) << std::endl;

        /*
        // Reflect and list the non-static data members
        [: expand(std::meta::nonstatic_data_members_of(user_reflection)) :] >>
        [&]<std::meta::info member>() {
            std::cout << " Member: " << std::meta::identifier_of(member) << std::endl;
        };
        */
    }
}

namespace reflection::get_data_member_1
{
    consteval auto data_members_of(const std::meta::info& info)
    {
        constexpr auto ctx = std::meta::access_context::current();
        return std::define_static_array(std::meta::nonstatic_data_members_of(info, ctx));
    }

    void demo()
    {
        template for( auto constexpr m: data_members_of(^^User) ) {   //***
            println( "member named: {}", std::meta::display_string_of(m) );
        }

        /*
        member named: reflection::User::name
        member named: reflection::User::age
        */
    }
}

namespace reflection::enums
{
    void printValues_Simple()
    {
        template for(constexpr auto color : std::define_static_array(std::meta::enumerators_of(^^Color)))
        {
            constexpr std::string_view colorName = std::meta::identifier_of(color);
            std::println("{}", colorName);
        }

        /*
        Green
        Blue
        Yellow
        Purple
        */
    }
}

namespace reflection::enums
{
    template<typename E>
    requires std::is_enum_v<E>
    consteval auto enumerators_of()
    {
        return []<size_t ... I>(std::index_sequence<I...>)
        {
            return std::array{[:enumerators_of(^^E)[I]:] ...};
        }
        (std::make_index_sequence<enumerators_of(^^E).size()>{});
    }

    template<typename E>
    requires std::is_enum_v<E>
    consteval auto identifiers_of()
    {
        return []<size_t ... I>(std::index_sequence<I...>)
        {
            return std::array{std::string_view(identifier_of(enumerators_of(^^E)[I])) ...};
        }
        (std::make_index_sequence<enumerators_of(^^E).size()>{});
    }

    template<typename E>
        requires std::is_enum_v<E>
    const auto& to_string(E value)
    {
        static const auto toStringMap = [] static
        {
            static constexpr auto AllValues = enumerators_of<E>();
            static constexpr auto AllStrings = identifiers_of<E>();
            static std::unordered_map<E, const std::string> result;
            for(const auto& [enumValue, string] : std::ranges::zip_view(AllValues, AllStrings))
                result.emplace(enumValue, string);
            return result;
        }();
        return toStringMap.at(value);
    }

    template<typename E>
    requires std::is_enum_v<E>
    std::ostream& operator<<(std::ostream& s, E value)
    {
        s << to_string(value);
        return s;
    }

    void printValues_Complex()
    {
        for(const auto color : enumerators_of<Color>())
            std::cout << color << std::endl;

        std::cout << std::endl;

        for(const auto animal : enumerators_of<Animal>())
            std::cout << animal << std::endl;

        /*
        Red
        Green
        Blue
        Yellow
        Purple

        Cat
        Dog
        Horse
        Rabbit
        Snail
        */
    }
}

namespace reflection::enums
{
    enum class Color
    {
        Transparent,
        Red = 2,
        Green,
        Blue = 8,
        Yellow
    };

    // https://compiler-explorer.com/z/W8b9xrx5j
    // https://andreasfertig.com/blog/2025/08/cpp26-reflection-at-compile-time/?trk=feed_main-feed-card_feed-article-content

    template<typename E>
    requires std::is_enum_v<E>
    constexpr inline auto num_enumerators_of {
        std::meta::enumerators_of(^^E).size()
    };

    template<typename E>
    requires std::is_enum_v<E>
    consteval auto get_enum_values() -> decltype(auto)
    {
        std::array<E, num_enumerators_of<E>> res;
        template for(size_t i{}; constexpr auto& e : std::define_static_array(std::meta::enumerators_of(^^E)))
        {
            res[i++] = [:e:];
        }
        return res;
    }

    void enumerateValues()
    {
        for(const auto e : get_enum_values<Color>()) {
            std::println("{} ", std::to_underlying(e));
        }
    }
}

namespace reflection::checking_type
{
    class A;

    class B
    {
        int a;
    };

    void is_complete_type()
    {
        constexpr std::meta::info aInfo = ^^A;
        constexpr std::meta::info bInfo = ^^B;

        std::println("Is 'A' complete type: {}", std::meta::is_complete_type(aInfo));
        std::println("Is 'B' complete type: {}", std::meta::is_complete_type(bInfo));
    }
}


namespace reflection::splicers
{
    void type_Aliases()
    {
        constexpr auto r = ^^int;
        typename[:r:] x = 42;

        static_assert(std::is_same_v<decltype(x), int>);
    }
}

namespace reflection::serialization
{
    struct Point2D
    {
        double x { 0.0 };
        double y { 0.0 };
    };

    template <typename T>
    void format(T const& t)
    {
        std::cout << identifier_of(^^T) << "{ ";
        constexpr auto access_ctx = std::meta::access_context::unchecked();

        template for (constexpr auto mem : std::define_static_array(nonstatic_data_members_of(^^T, access_ctx))) {
            std::cout << std::format("{} = {}, ", identifier_of(mem), t.[:mem:]);
        }

        std::cout << "}\n";
    }

    void simpleType()
    { 
        format(Point2D{.x = 3.0, .y = 4.0}); 
    }
}



int main(const int argc,
         char** argv,
         char** environment)
{
    using namespace reflection;

    // types::basics();
    // types::reflectTypes_Simple();
    // types::useReflectedData();
    // types::deduce_Type_of_Vector<int>();
    // types::deduce_Type_of_Vector<double>();
    types::Create_New_Type();

    // get_data_member_0::retrieve_Data_Members_Names();
    // get_data_member_1::demo();

    // enums::printValues_Simple();
    // enums::printValues_Complex();
    // enums::enumerateValues();

    // checking_type::is_complete_type();

    // splicers::type_Aliases();

    // serialization::simpleType();

    return EXIT_SUCCESS;
}

