#include "commands.h"

void InitCommand::execute(RelaisDriver* driver)
{
    driver->initDevice();
}

void NopCommand::execute(RelaisDriver* driver)
{
    driver->nop();
}

void SetCommand::execute(RelaisDriver* driver)
{
    driver->setPortsImmediatly(ports);
}

void UnsetCommand::execute(RelaisDriver* driver)
{
    driver->unsetPortsImmediatly(ports);
}

void ToggleCommand::execute(RelaisDriver* driver)
{
    driver->togglePortsImmediatly(ports);
}

