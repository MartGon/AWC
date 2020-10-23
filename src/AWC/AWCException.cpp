#include <AWC/AWCException.h>
#include <Utils/Vector2.h>

#include <algorithm>

// AWCException - Base

AWCException::AWCException(std::string msg) : msg_{msg}
{

}

const char* AWCException::what() const noexcept
{
    return msg_.c_str();
}

// AWCInvalidIndexException

AWCInvalidIndexException::AWCInvalidIndexException(std::string formattedMsg, Vector2 index) : index{index}, AWCException{GetMessage(formattedMsg)}
{

}

AWCInvalidIndexException::AWCInvalidIndexException(std::string formattedMsg, int x, int y) : AWCInvalidIndexException(formattedMsg, Vector2{x, y})
{

}

std::string AWCInvalidIndexException::GetMessage(std::string formattedMsg) const
{
    auto tokenPos = formattedMsg.find(FORMAT_TOKEN);
    auto indexStr = index.ToString();
    formattedMsg.replace(tokenPos, FORMAT_TOKEN.size(), indexStr);

    return formattedMsg;
}

// NoExistingIndex

AWCNoExistingIndexException::AWCNoExistingIndexException(Vector2 index) 
    : AWCInvalidIndexException("Index %p did not exist", index)
{

}

AWCNoExistingIndexException::AWCNoExistingIndexException(int x, int y) 
    : AWCNoExistingIndexException(Vector2{x, y})
{

}

// Already existing

AWCAlreadyExistingIndexException::AWCAlreadyExistingIndexException(Vector2 index)
    : AWCInvalidIndexException("Index %p already exists", index)
{

}

AWCAlreadyExistingIndexException::AWCAlreadyExistingIndexException(int x, int y)  
    : AWCAlreadyExistingIndexException(Vector2{x, y})
{

}