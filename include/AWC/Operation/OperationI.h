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

    enum class Type
    {
        NONE,
        CUSTOM,
        COMPOSED,
        STAT_MOD,
        UPDATE_FLAG,
        MOVE
    };

    class OperationI
    {
    public:
        OperationI(Type type = Type::NONE) : type_{type} {};
        virtual ~OperationI() {};
        virtual Result Execute(Game& state) { return Result{SUCCESS}; };

        Type GetType() { return type_; }

        template <typename T>
        T* To()
        {
            static_assert(std::is_base_of<OperationI, T>::value, "Derived not derived from BaseClass");
            return static_cast<T*>(this);
        }

    private:
        Type type_;
    };
}