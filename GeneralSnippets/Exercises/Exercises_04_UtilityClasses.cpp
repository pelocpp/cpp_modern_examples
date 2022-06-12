// =====================================================================================
// Exercises_04_UtilityClasses.cpp
// =====================================================================================

#include <iostream>
#include <string>
#include <vector>
#include <optional>
#include <variant>
#include <type_traits>

namespace Exercises_UtilityClasses {

    namespace Exercise_01 {

        // generic visitor (matching all types in the variant)
        auto visitor = [](auto const& elem) {
            std::cout << elem << std::endl;
        };

        void testExercise_01a() {

            std::variant<int, double, std::string> var{ 123.456 };

            std::visit(visitor, var);

            var = 10;
            std::visit(visitor, var);

            var = std::string{ "ABCDE" };
            std::visit(visitor, var);
        }

        // improved generic visitor
        // note: doesn't compile with gcc :(
        auto improvedVisitor = [](auto const& elem) {

            using T = std::remove_reference<decltype(elem)>::type;

            if constexpr (std::is_scalar<T>::value)
            {
                    if constexpr (std::is_same<int, std::remove_cvref<T>::type> ::value)
                    {
                        std::cout << "int: " << elem << std::endl;
                    }
                    else if constexpr (std::is_same<double, std::remove_cvref<T>::type> ::value)
                    {
                        std::cout << "double: " << elem << std::endl;
                    }
            }
            else
            {
                std::cout << "std:string: " << elem << std::endl;
            }
        };

        void testExercise_01b() {

            std::variant<int, double, std::string> var{ 123.456 };

            std::visit(improvedVisitor, var);

            var = 10;
            std::visit(improvedVisitor, var);

            var = std::string{ "ABCDE" };
            std::visit(improvedVisitor, var);
        }

        void testExercise_01() {
            testExercise_01a();
            testExercise_01b();
        }
    }

    namespace Exercise_02 {

        std::optional<int> toInt(std::string s)
        {
            std::optional<int> result{ std::nullopt };
            
            try
            {
                int i = std::stoi(s);

                // want input string to be consumed entirely (there are several ways
                // to accomplish this each with advantages and disadvantages)
                std::string tmp = std::to_string(i);
                if (tmp.length() != s.length())
                    throw std::invalid_argument("input string illegal");

                result.emplace(i);
            }
            catch (const std::invalid_argument&)
            {
                std::cerr << "Bad input: std::invalid_argument" << std::endl;
            }
            catch (const std::out_of_range&)
            {
                std::cerr << "Integer overflow: std::out_of_range" << std::endl;
            }

            return result;
        }

        template <typename T>
        std::optional<T> toNumber(std::string s)
        {
            std::optional<T> result{ std::nullopt };

            try
            {
                if constexpr (std::is_same_v<T, int> || std::is_same_v<T, short>) {
                    int i = std::stoi(s);
                    std::string tmp = std::to_string(i);
                    if (tmp.length() != s.length())
                        throw std::invalid_argument("input string illegal");

                    result.emplace(i);
                }
                if constexpr (std::is_same_v<T, long>) {
                    long l = std::stol(s);
                    std::string tmp = std::to_string(l);
                    if (tmp.length() != s.length())
                        throw std::invalid_argument("input string illegal");

                    result.emplace(l);
                }
                if constexpr (std::is_same_v<T, long long>) {
                    long long ll = std::stoll(s);
                    std::string tmp = std::to_string(ll);
                    if (tmp.length() != s.length())
                        throw std::invalid_argument("input string illegal");

                    result.emplace(ll);
                }
            }
            catch (const std::invalid_argument&)
            {
                std::cerr << "Bad input: std::invalid_argument" << std::endl;
            }
            catch (const std::out_of_range&)
            {
                std::cerr << "Integer overflow: std::out_of_range" << std::endl;
            }

            return result;
        }

        void testExercise_01a() {
            std::optional<int> i1 = toInt("123");
            if (i1.has_value()) {
                std::cout << i1.value() << std::endl;
            }

            std::optional<int> i2 = toInt("-987654321");
            if (i2.has_value()) {
                std::cout << i2.value() << std::endl;
            }

            std::optional<int> i3 = toInt("123ABC");
            if (i3.has_value()) {
                std::cout << i3.value() << std::endl;
            }

            std::optional<int> i4 = toInt("ABC123");
            if (i4.has_value()) {
                std::cout << i4.value() << std::endl;
            }
        }

        void testExercise_01b() {

            std::optional<short> i1 = toNumber<short>("32767");
            if (i1.has_value()) {
                std::cout << i1.value() << std::endl;
            }

            std::optional<int> i2 = toNumber<int>("2147483647");
            if (i2.has_value()) {
                std::cout << i2.value() << std::endl;
            }

            std::optional<long int> i3 = toNumber<long int>("2147483647");
            if (i3.has_value()) {
                std::cout << i3.value() << std::endl;
            }

            std::optional<long long> i4 = toNumber<long long>("9223372036854775807");
            if (i4.has_value()) {
                std::cout << i4.value() << std::endl;
            }
        }

        void testExercise_01() {
            testExercise_01a();
            testExercise_01b();
        }
    }

    namespace Exercise_03 {

        // https://gieseanw.wordpress.com/2017/05/03/a-true-heterogeneous-container-in-c/

        void testExercise_01a()
        {
            std::variant<int, std::string> myVariant;
            myVariant = 123;
            std::cout << std::get<int>(myVariant) << std::endl;
            myVariant = std::string("456");
            std::cout << std::get<std::string>(myVariant) << std::endl;
        }

        struct MyPrintVisitor
        {
            void operator()(const int& value) { std::cout << "int: " << value << std::endl; }
            void operator()(const std::string& value) { std::cout << "std::string: " << value << std::endl; }
        };

        // oder

        struct AllInOneVisitor
        {
            template <class T>
            void operator()(const T& value) { std::cout << value << std::endl; }
        };

        // oder

        struct MyModifyingVisitor
        {
            template <class T>
            void operator()(T& value) { value += value; }
        };

        auto lambdaAllInOneVisitor = [](const auto& value) {
            std::cout << value << std::endl;
        };

        void testExercise_01b()
        {
            std::variant<int, std::string> myVariant;
            myVariant = 123;
            std::visit(MyPrintVisitor{}, myVariant);
            myVariant = std::string("456");
            std::visit(MyPrintVisitor{}, myVariant);

            myVariant = 123;
            std::visit(AllInOneVisitor{}, myVariant);
            myVariant = std::string("456");
            std::visit(AllInOneVisitor{}, myVariant);

            myVariant = 123;
            std::visit(lambdaAllInOneVisitor, myVariant);
            myVariant = std::string("456");
            std::visit(lambdaAllInOneVisitor, myVariant);

            myVariant = 123;
            std::visit(MyModifyingVisitor{}, myVariant);
            std::visit(MyPrintVisitor{}, myVariant);
            myVariant = std::string("456");
            std::visit(MyModifyingVisitor{}, myVariant);
            std::visit(MyPrintVisitor{}, myVariant);
        }

        void testExercise_01c()
        {
            std::vector<std::variant<int, std::string>> hetVec;

            hetVec.emplace_back(12);
            hetVec.emplace_back(std::string("34"));
            hetVec.emplace_back(56);
            hetVec.emplace_back(std::string("78"));

            // print them
            for (const auto& var : hetVec) {
                std::visit(MyPrintVisitor{}, var);
            }

            // modify them
            for (auto& var : hetVec) {
                std::visit(MyModifyingVisitor{}, var);
            }

            // print them again
            for (const auto& var : hetVec) {
                std::visit(MyPrintVisitor{}, var);
            }
        }

        template <typename... T>
        class HeterogeneousContainer
        {
        private:
            std::vector<std::variant<T...>> m_values;

        public:
            // visitor
            template <class V>
            void visit(V&& visitor) {
                for (auto& value : m_values) {
                    std::visit(visitor, value);
                }
            }

            // accessor
            std::vector<std::variant<T...>>& Values() {
                return m_values;
            };
        };

        void testExercise_01d()
        {
            HeterogeneousContainer<int, std::string> hetCont;

            hetCont.Values().emplace_back(12);
            hetCont.Values().emplace_back(std::string("34"));
            hetCont.Values().emplace_back(56);
            hetCont.Values().emplace_back(std::string("78"));

            // print them
            hetCont.visit(lambdaAllInOneVisitor);
            std::cout << std::endl;

            // modify them
            hetCont.visit(MyModifyingVisitor{});

            // print again
            hetCont.visit(lambdaAllInOneVisitor);
            std::cout << std::endl;
        }

        void testExercise_01() {
            testExercise_01a();
            testExercise_01b();
            testExercise_01c();
            testExercise_01d();
        }
    }
}

void test_exercises_utility_classes()
{
    using namespace Exercises_UtilityClasses;
    Exercise_01::testExercise_01();
    Exercise_02::testExercise_01();
    Exercise_03::testExercise_01();
}

// =====================================================================================
// End-of-File
// =====================================================================================
