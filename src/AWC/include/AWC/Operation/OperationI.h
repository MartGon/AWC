#pragma once
#include <AWC/AWCusing.h>

namespace Operation
{
    enum ResultCode
    {
        SUCCESS,
        ERROR
    };

    class Result
    {
    public:

        Result(ResultCode code) : code_{code} {}
        explicit operator bool() const { return code_ == SUCCESS;}

    private:

        ResultCode code_;
    };

    enum Type : unsigned int
    {
        NONE,
        CUSTOM,
        COMPOSED,
        ANTI_OPERATION,
        
        STAT_MOD,
        UPDATE_FLAG,
        MOVE,
        ATTACK,
        TAKE_DMG
    };

    class OperationI
    {
    public:
        OperationI(unsigned int type = Type::NONE) : type_{type} {};
        virtual ~OperationI() {};
        virtual Result Execute(Game& state, uint8_t prio) { return Result{SUCCESS}; };
    
        unsigned int GetType() { return type_; }

        template <typename T>
        T* To()
        {
            static_assert(std::is_base_of<OperationI, T>::value, "Derived not derived from BaseClass");
            return static_cast<T*>(this);
        }

    private:
        unsigned int type_;
    };
}