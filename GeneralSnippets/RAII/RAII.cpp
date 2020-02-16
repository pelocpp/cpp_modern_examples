// =====================================================================================
// RAII
// =====================================================================================

#include <iostream>

#include "..\Global\Dummy.h"

namespace RAIIDemo {

    template <typename T>
    class RAII {
    public:
        explicit RAII (T* p) : m_p(p) {}
        ~RAII() { delete m_p; };

        // grant access to pointer inside RAII object
        T* operator->() { return m_p; }
        const T* operator->() const { return m_p; }
        T& operator&() { return *m_p; }
        const T* operator&() const { return *m_p; }

    private:
        T* m_p;
    };

    void test_01 () {

        {
            RAII<Dummy> p (new Dummy (1));
        }

        std::cout << "Done." << std::endl;
    }

    void test_02() {

        {
            RAII<Dummy> p (new Dummy(2));
            p->sayHello();
            int value = p->getValue();
            std::cout << "Value " << value << " inside Dummy object." << std::endl;

            Dummy& dRef = p.operator&();
            dRef.sayHello();
            value = dRef.getValue();
            std::cout << "Value " << value << " inside Dummy object." << std::endl;
        }

        std::cout << "Done." << std::endl;
    }

    void test_03() {

        // test RAII idiom upon a loop break
        do {
            RAII<Dummy> p(new Dummy(3));
            break;
        } while (false);

        std::cout << "Done." << std::endl;
    }

    void test_04() {

        // test RAII idiom upon exception being thrown
        try {
            RAII<Dummy> p(new Dummy(4));
            throw 99;
        } 
        catch (int n) {
            std::cout << "Exception " << n << " occurred!" << std::endl;
        }

        std::cout << "Done." << std::endl;
    }

    class RAIIContainer {
    public:
        RAIIContainer(Dummy* p) : m_rp(p) {}

    private:
        RAII<Dummy> m_rp;
    };

    void test_05() {

        {
            RAIIContainer cont (new Dummy(5));
        }

        std::cout << "Done." << std::endl;
    }
}

int main_raii()
{
    using namespace RAIIDemo;
    test_01();
    test_02();
    test_03();
    test_04();
    test_05();
    return 0;
}

// =====================================================================================
// End-of-File
// =====================================================================================
