// =====================================================================================
// Variadic Templates
// =====================================================================================

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <type_traits>

// =====================================================================================
// Inhalt:
// - Variadic Expressions / Variadische Ausdr�cke
// - Variadische Templates mit Zugriff auf Basisklasse und 'using'
// - Variadic Expressions with Indices / Variadische Ausdr�cke mit Indizes
// - Compile-Time Expression with Variadic Expressions
// =====================================================================================

namespace VariadicTemplatesSystematic {

    // Use case: Variadic Expressions / Variadische Ausdr�cke
    //

    template<typename T>
    void print(T arg)
    {
        std::cout << arg << '\n'; // print passed argument
    }

    template<typename T, typename... Types>
    void print(T firstArg, Types... args)
    {
        print(firstArg); // call print() for first argument
        print(args...);  // call print() for remaining arguments
    }

    template<typename... T>
    void printDoubled(T const&... args)
    {
        print((args + args)...);
        // same as
        print(args + args...);
    }

    template<typename... T>
    void printFiveTimes(T const&... args)
    {
        print(5 * args...);
    }

    template<typename... T>
    void printPlusOne(T const&... args)
    {
        print(args  +1 ...);    // works
        // print(args + 1...);  // doesn't compile: integer literal with too many decimal points
        // print((args + 1) ...);  // works
    }

    void test_01()
    {
        printDoubled(7.5);

        printFiveTimes(1, 2, 3, 4, 5);

        printPlusOne(1, 5, 10);
    }

    // =====================================================================================
    // =====================================================================================

    // Use case: Variadic Base Classes and keyword 'using'
    // Several implementations of a method (e.g. operator())
    // can be reached from the corresponding base class 

    class Customer
    {
    private:
        std::string m_name;
    public:
        Customer(const std::string& name) : m_name(name) { }
        std::string getName() const { return m_name; }
    };

    struct CustomerEquals {
        bool operator() (const Customer& c1, const Customer& c2) const {
            std::cout << "isEqual" << std::endl;
            return c1.getName() == c2.getName();
        }
    };

    struct CustomerHash {
        std::size_t operator() (Customer const& c) const {
            std::cout << "hash" << std::endl;
            return std::hash<std::string>()(c.getName());
        }
    };

    template<typename... RULES>
    struct CustomerRules : RULES...
    {
        using RULES::operator() ... ; // C++17
    };

    void test_02()
    {
        using Rules = CustomerRules<CustomerEquals, CustomerHash>;

        Rules rules;

        Customer hans("Hans");
        Customer sepp("Sepp");

        bool result = rules(hans, sepp);
        std::cout << "rules(hans, sepp) => " << result << std::endl;

        size_t hashHans = rules(hans);
        std::cout << "rules(hans) => " << hashHans << std::endl;

        size_t hashSepp = rules(sepp);
        std::cout << "rules(sepp) => " << hashSepp << std::endl;
    }

    // =====================================================================================
    // =====================================================================================

    // Use case: Variadic Expressions with Indices / Variadische Ausdr�cke mit Indizes
    //
    
    template <typename T, typename ...IDX>
    void printElements(const T& container, IDX ...index) {
    
        print(container[index] ...);
    }
    
    void test_03()
    {
        std::vector<std::string> numbers{ "one", "two", "three", "four", "five" };
        printElements(numbers, 4, 3, 2);
    
        std::string s{ "0123456789" };
        printElements(s, 8, 6, 4, 2, 0);
    }

    // =====================================================================================
    // =====================================================================================

    // Use case: Compile-Time Expression with Variadic Expressions
    //

    //template<typename T, typename... TREST>
    //constexpr bool isSameType(T, TREST...)
    //{
    //    return (std::is_same<T, TREST>::value && ...); // since C++17: folding expression !!!
    //}

    //template<typename T, typename... TREST>
    //constexpr bool isSameType(T, TREST...)
    //{
    //    return (std::is_same<decltype(T), decltype(TREST)>::value && ...); // since C++17: folding expression !!!
    //}

    template<typename T1, typename T2>
    void isSameType(T1 arg)
    {
        return std::is_same<decltype(T1), decltype(T2)>::value;
    }

    template<typename T1, typename... TREST>
    void isSameType(T1 firstArg, TREST... args)
    {
        return (std::is_same<T1, TREST>::value && ...);
    }


    void test_04()
    {
        //bool result = isSameType(43, false, "hello");
        //std::cout << std::boolalpha << result << std::endl;

        // expands to: 

        //result = std::is_same<int, int>::value && std::is_same<int, char const*>::value;
        //std::cout << std::boolalpha << result << std::endl;

        //result = isSameType(123, 456, 789);
        //std::cout << std::boolalpha << result << std::endl;

        //// expands to: 

        //result = std::is_same<int, int>::value && std::is_same<int, int>::value;
        //std::cout << std::boolalpha << result << std::endl;
    }
}

// =====================================================================================
// =====================================================================================

int main_variadic_templates()
{
    using namespace VariadicTemplatesSystematic;

    // Variadic Expressions
    test_01();

    // Variadic Base Classes and keyword 'using'
    test_02();

    // Variadic Expressions with Indices 
    test_03();

    // Compile-Time Expression with Variadic Expressions
    test_04();

    return 0;
}

// =====================================================================================
// End-of-File
// =====================================================================================
