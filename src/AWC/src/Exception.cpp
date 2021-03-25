#include <AWC/AWCException.h>

#include <Utils.h>


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

const std::string AWCInvalidIndexException::INDEX_TOKEN = "%i";
const std::string AWCInvalidIndexException::PREFIX_TOKEN = "%p";

AWCInvalidIndexException::AWCInvalidIndexException(std::string formattedMsg, std::string prefix, Vector2 pos) 
    : index{pos}, AWCException{GetMessage(formattedMsg, prefix, pos)}
{
    
}

AWCInvalidIndexException::AWCInvalidIndexException(std::string formattedMsg, std::string prefix, int x, int y) 
    : index{Vector2{x, y}}, AWCException{GetMessage(formattedMsg, prefix, {x, y})}
{
    
}

AWCInvalidIndexException::AWCInvalidIndexException(std::string formattedMsg, Vector2 pos) 
    : index{pos}, AWCException{GetMessage(formattedMsg, std::string{}, pos)}
{

}

AWCInvalidIndexException::AWCInvalidIndexException(std::string formattedMsg, int x, int y) 
    : AWCInvalidIndexException(formattedMsg, Vector2{x, y})
{

}

std::string AWCInvalidIndexException::GetMessage(std::string formattedMsg, std::string prefix, Vector2 index) const
{
    // Replace index
    formattedMsg = StringUtils::Replace(formattedMsg, INDEX_TOKEN, index.ToString());
    formattedMsg = StringUtils::Replace(formattedMsg, PREFIX_TOKEN, prefix);

    return formattedMsg;
}

// NoExistingIndex

AWCNoExistingIndexException::AWCNoExistingIndexException(std::string prefix, Vector2 index) 
    : AWCInvalidIndexException("%p: Index %i did not exist", prefix, index)
{

}

AWCNoExistingIndexException::AWCNoExistingIndexException(std::string prefix, int x, int y) 
    : AWCNoExistingIndexException(prefix, Vector2{x, y})
{

}

AWCNoExistingIndexException::AWCNoExistingIndexException(Vector2 index) 
    : AWCInvalidIndexException("Index %i did not exist", index)
{

}

AWCNoExistingIndexException::AWCNoExistingIndexException(int x, int y) 
    : AWCNoExistingIndexException(Vector2{x, y})
{

}

// Already existing

AWCAlreadyExistingIndexException::AWCAlreadyExistingIndexException(std::string prefix, Vector2 index)
    : AWCInvalidIndexException("%p: Index %i already exists", prefix, index)
{

}

AWCAlreadyExistingIndexException::AWCAlreadyExistingIndexException(std::string prefix, int x, int y)
    : AWCAlreadyExistingIndexException(prefix, Vector2{x, y})
{

}

AWCAlreadyExistingIndexException::AWCAlreadyExistingIndexException(Vector2 index) 
    : AWCInvalidIndexException("Index %i already exists", index)
{

}

AWCAlreadyExistingIndexException::AWCAlreadyExistingIndexException(int x, int y)  
    : AWCAlreadyExistingIndexException(Vector2{x, y})
{

}