#include <Script/ScriptOperation.h>
#include <Script/ScriptType.h>


Operation::Result Script::ScriptOperation::Execute(::Game& state, uint8_t prio)
{
    return scriptType_.Execute(state, prio, argsTable_.GetRef());
}