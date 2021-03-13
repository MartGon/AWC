#include <Script/ScriptOperation.h>
#include <Script/ScriptType.h>

const unsigned int Script::SCRIPT = 9;
const char* Script::MT_NAME = "_MAIN";

Operation::Result Script::ScriptOperation::Execute(::Game& state, uint8_t prio)
{
    return scriptType_.Execute(state, prio, argsTable_);
}