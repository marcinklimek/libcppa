#include <iostream>
#include "cppa/cppa.hpp"
#include "cppa/to_string.hpp"

using std::cout;
using std::endl;
using std::make_pair;

using namespace cppa;

// the foo class from example 3
class foo
{

    int m_a;
    int m_b;

 public:

    foo() : m_a(0), m_b(0) { }

    foo(int a0, int b0) : m_a(a0), m_b(b0) { }

    foo(const foo&) = default;

    foo& operator=(const foo&) = default;

    int a() const { return m_a; }

    void set_a(int val) { m_a = val; }

    int b() const { return m_b; }

    void set_b(int val) { m_b = val; }

};

// needed for operator==() of bar
bool operator==(const foo& lhs, const foo& rhs)
{
    return    lhs.a() == rhs.a()
           && lhs.b() == rhs.b();
}

// simple struct that has foo as a member
struct bar
{
    foo f;
    int i;
};

// announce requires bar to have the equal operator implemented
bool operator==(const bar& lhs, const bar& rhs)
{
    return    lhs.f == rhs.f
           && lhs.i == rhs.i;
}

// "worst case" class ... not a good software design at all ;)
class baz
{

    foo m_f;

 public:

    bar b;

    // announce requires a default constructor
    baz() = default;

    inline baz(const foo& mf, const bar& mb) : m_f(mf), b(mb) { }

    const foo& f() const { return m_f; }

    void set_f(const foo& val) { m_f = val; }

};

// even worst case classes have to implement operator==
bool operator==(const baz& lhs, const baz& rhs)
{
    return    lhs.f() == rhs.f()
           && lhs.b == rhs.b;
}

int main(int, char**)
{
    // bar has a non-trivial data member f, thus, we have to told
    // announce how to serialize/deserialize this member;
    // this is was the compound_member function is for;
    // it takes a pointer to the non-trivial member as first argument
    // followed by all "sub-members" either as member pointer or
    // { getter, setter } pair
    announce<bar>(compound_member(&bar::f,
                                  make_pair(&foo::a, &foo::set_a),
                                  make_pair(&foo::b, &foo::set_b)),
                  &bar::i);

    // baz has non-trivial data members with getter/setter pair
    // and getter returning a mutable reference
    announce<baz>(compound_member(make_pair(&baz::f, &baz::set_f),
                                  make_pair(&foo::a, &foo::set_a),
                                  make_pair(&foo::b, &foo::set_b)),
                  // compound member that has a compound member
                  compound_member(&baz::b,
                                  compound_member(&bar::f,
                                                  make_pair(&foo::a, &foo::set_a),
                                                  make_pair(&foo::b, &foo::set_b)),
                                  &bar::i));

    // send a bar to ourselves
    send(self, bar{foo{1,2},3});
    // send a baz to ourselves
    send(self, baz{foo{1,2},bar{foo{3,4},5}});

    // receive two messages
    int i = 0;
    receive_for(i, 2)
    (
        on<bar>() >> [](bar const& val)
        {
            cout << "bar(foo("
                 << val.f.a() << ","
                 << val.f.b() << "),"
                 << val.i << ")"
                 << endl;
        },
        on<baz>() >> [](baz const& val)
        {
            // prints: baz ( foo ( 1, 2 ), bar ( foo ( 3, 4 ), 5 ) )
            cout << to_string(val) << endl;
        }
    );
    return 0;
}

