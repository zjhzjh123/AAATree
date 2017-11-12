#ifndef SJTU_EXCEPTIONS_HPP
#define SJTU_EXCEPTIONS_HPP

#include <cstddef>
#include <cstring>
#include <string>

namespace sjtu {
    
    class exception {
    protected:
        const std::string variant;
        std::string detail;
    public:
        exception() {}
        exception(std::string a, std::string b):variant(a),detail(b){}
        exception(const exception &ec) : variant(ec.variant), detail(ec.detail) {}
        virtual std::string what() {
            return variant + " " + detail;
        }
        ~exception(){}
    };
    
    /**
     * TODO
     * Please complete them.
     */
    class index_out_of_bound : public exception {
        index_out_of_bound():exception("invalid..", "invalid..."){}
    };
    
    class runtime_error : public exception {
    public:
        runtime_error():exception("invalid..", "invalid..."){}
    };
    
    class invaild_iterator : public exception {
    public:
        invaild_iterator (): exception("invalid..", "invalid..."){}
    };
    
    class container_is_empty : public exception {
    public:
        container_is_empty():exception("invalid..", "invalid..."){}
    };
}

#endif