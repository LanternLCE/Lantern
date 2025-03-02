#include "pch.h"
#include "Minecraft.h"

MINECRAFT_EVENT Event* Minecraft::onMain = new Event();

int Minecraft::main(int _Argc, char** _Argv, char** _Env)
{
    onMain->fire();
    return Minecraft_main(_Argc, _Argv, _Env);
}
