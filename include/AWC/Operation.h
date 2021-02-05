#pragma once

#include <AWC/AWCusing.h>
#include <AWC/Unit/Unit.h>

#include <string>
#include <functional>

class Game;

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
        UPDATE_FLAG
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

    class Custom : public OperationI
    {
    public:
        ~Custom() override {};
        Custom(std::function<void(Game&)> func) : func_{func}, OperationI{Type::CUSTOM} {};
        Result Execute(Game& state) override { func_(state); return Result{SUCCESS}; }
    private:
        std::function<void(Game&)> func_;
    };

    enum class ComposedType
    {

    };

    class Composed : public OperationI
    {
    public:
        ~Composed() override {
            for(auto child : children)
                delete child;
        };
        Composed() : OperationI{Type::COMPOSED} {};

        void AddChild(OperationI* child) { children.push_back(child); };

        Result Execute(Game& state) override;

    private:
        std::vector<OperationI*> children;
    };

    class StatMod : public OperationI
    {
    public:
        ~StatMod() override {};
        StatMod(UnitPtr unit, UnitNS::StatType type, int amount) : unit_{unit}, type_{type}, amount_{amount}, OperationI{Type::STAT_MOD} {};

        Result Execute(Game& state) override;
    
        UnitPtr unit_;
        UnitNS::StatType type_;
        int amount_;
    };

    class UpdateFlag : public OperationI
    {
    public:
        ~UpdateFlag() override {};
        UpdateFlag(UnitPtr unit, UnitNS::Flag flag, bool set) : unit_{unit}, flag_{flag}, set_{set}, OperationI{Type::UPDATE_FLAG} {};

        Result Execute(Game& game) override;
    
        UnitPtr unit_;
        UnitNS::Flag flag_;
        bool set_;
    };
}