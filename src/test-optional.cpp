#include <iostream>
#include <string>

#include "util/Optional.hpp"

using namespace std;
using namespace util;


#define LOG(MSG) (cout << __func__ << ", L" << __LINE__ << ": " << MSG << endl)

#define OBJ_LOG(MSG) LOG("@ " << this << ": " << MSG)


class BaseClass
{
public:
    
    explicit BaseClass(string const& s): _s(s) {
        OBJ_LOG("Creation pour " << _s);
    }
    
    BaseClass(BaseClass const& src): _s(src._s) {
        OBJ_LOG("Copie par constructeur pour " << &src);
    }
    
    BaseClass(BaseClass&& src): _s(move(src._s)) {
        OBJ_LOG("Deplacement par constructeur de " << &src);
    }
    
    virtual ~BaseClass() {
        OBJ_LOG("Destruction");
    }
    
    BaseClass& operator =(BaseClass const& src) {
        OBJ_LOG("Copie par operateur pour " << &src);
        _s = src._s;
        return *this;
    }
    
    BaseClass& operator =(BaseClass&& src) {
        OBJ_LOG("Deplacement par operateur de " << &src);
        _s = move(src._s);
        return *this;
    }
    
    friend ostream& operator <<(ostream& out, BaseClass const& bc);

protected:
    
    string _s;
    
};

ostream& operator <<(ostream& out, BaseClass const& bc) {
    return out << "BaseClass(\"" << bc._s << "\") @ " << &bc;
}


class DerivedClass: public BaseClass
{
public:
    
    explicit DerivedClass(string const& s): BaseClass(s) {
        OBJ_LOG("Creation pour " << s);
    }
    
    DerivedClass(DerivedClass const& src): BaseClass(src) {
        OBJ_LOG("Copie par constructeur pour " << &src);
    }
    
    DerivedClass(DerivedClass&& src): BaseClass(move(src)) {
        OBJ_LOG("Deplacement par constructeur de " << &src);
    }
    
    virtual ~DerivedClass() {
        OBJ_LOG("Destruction");
    }
    
    DerivedClass& operator =(DerivedClass const& src) {
        OBJ_LOG("Copie par operateur pour " << &src);
        BaseClass::operator =(src);
        return *this;
    }
    
    DerivedClass& operator =(DerivedClass&& src) {
        OBJ_LOG("Deplacement par operateur de " << &src);
        BaseClass::operator =(move(src));
        return *this;
    }
    
    explicit operator string() const {
        OBJ_LOG("Conversion en string pour " << this);
        OBJ_LOG("Conversion en string pour " << &_s);
        return _s;
    }
    
    friend ostream& operator <<(ostream& out, DerivedClass const& bc);
    
};

ostream& operator <<(ostream& out, DerivedClass const& dc) {
    return out << "DerivedClass(" << reinterpret_cast<BaseClass const&>(dc) << ")";
}


/*
void testBasicTypes() {
    LOG("Test basic types");
    
    {
        Optional<long> a1;
        Optional<long> a2(6);
        
        LOG("a1 = " << a1);
        LOG("a2 = " << a2);
        
        Optional<int> b1(a1);
        Optional<int> b2(a2);
        
        LOG("b1 = " << b1);
        LOG("b2 = " << b2);
    }
}
*/


void testDerivedTypes() {
    LOG("Test derived types");
    
    {
        Optional<DerivedClass> a1;
        Optional<DerivedClass> a2(DerivedClass("This is us"));
        
        LOG("a1 = " << a1);
        LOG("a2 = " << a2);
        
        LOG("Creation vers la base");
        Optional<BaseClass> b1;
        Optional<BaseClass> b2(a2);
        
        LOG("b1 = " << b1);
        LOG("b2 = " << b2);
        
        LOG("Copie : b1 = b2");
        b1 = b2;
        
        LOG("b1 = " << b1);
        LOG("b2 = " << b2);
        
        LOG("Reset a2");
        
        a2.reset();
        
        LOG("a1 = " << a1);
        LOG("a2 = " << a2);
        
        LOG("Reset and set b1");
        
        b1.reset();
        b1 = BaseClass("Pierre");
        
        LOG("b1 = " << b1);
        LOG("b2 = " << b2);
        
        LOG("Set b1");
        
        b1 =  Optional<BaseClass>();
        b1 =  Optional<BaseClass>();
        
        LOG("b1 = " << b1);
        LOG("b2 = " << b2);
        
        LOG("Set b1");
        
        b1 =  Optional<BaseClass>(BaseClass("123"));
        
        LOG("b1 = " << b1);
        LOG("b2 = " << b2);
        
        LOG("Swap b1 & b2");
        swap(b1, b2);
        
        LOG("b1 = " << b1);
        LOG("b2 = " << b2);
        
        LOG("Set a1");
        
        a1 = DerivedClass("xyz");
        
        LOG("a1 = " << a1);
        LOG("a2 = " << a2);
        
        LOG("To String");
        
        Optional<string> c1 = static_cast<Optional<string>>(static_cast<string>(*a1));
        Optional<string> c2 = static_cast<Optional<string>>(a2);
        
        LOG("a1 = " << a1);
        LOG("a2 = " << a2);
        
        LOG("c1 = " << c1);
        LOG("c2 = " << c2);
        
        LOG("Fin");
    }
}


int main() {
    // testBasicTypes();
    testDerivedTypes();
    return 0;
}
