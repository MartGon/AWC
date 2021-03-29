#include <Script/ScriptOperation.h>
#include <Script/ScriptType.h>

const unsigned int Script::SCRIPT = 9;
const char* Script::MT_NAME = "_MAIN";

Operation::Result Script::ScriptOperation::Execute(::Game& state, const Process::Process& p)
{
    return scriptType_.Execute(state, p, argsTable_);
}