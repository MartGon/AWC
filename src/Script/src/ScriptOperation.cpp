#include <Script/ScriptOperation.h>
#include <Script/ScriptType.h>

const unsigned int Script::SCRIPT = 9;
const char* Script::MT_NAME = "_MAIN";

Operation::Result Script::ScriptOperation::Execute(::Game& state, Process::Info info)
{
    return scriptType_.Execute(state, info, argsTable_);
}