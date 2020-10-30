#include <AWC/MovementDesc.h>
#include <AWC/MovementDescType.h>

MovementDesc::MovementDesc(const MovementDescType& moveType) : moveType{moveType}, currentGas{moveType.maxGas}
{

}