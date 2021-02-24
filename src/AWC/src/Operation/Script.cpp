#include <AWC/Operation/Script.h>
#include <AWC/Operation/ScriptType.h>

using namespace Operation;

Result Script::Execute(Game& state, uint8_t prio)
{
    return scriptType_.Execute(state, prio, argsTable_.GetRef());
}