#pragma once
#include <AWC/AWCusing.h>
#include <AWC/Process.h>

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
        Result(ResultCode code, std::string reason) : code_{code}, reason_{reason} {}
        explicit operator bool() const { return code_ == SUCCESS;}

        inline std::string GetReason() const
        {
            return reason_;
        }

    private:

        ResultCode code_;
        std::string reason_;
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
        OperationI(unsigned int type) : type_{type} {};
        virtual ~OperationI() {};
        virtual Result Execute(Game& state, const Process::Process& p) { return Result{SUCCESS}; };
        virtual void Undo(Game& state, const Process::Process& p) {};
    
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