#include <AWC/Exception.h>

#include <Utils.h>


#include <algorithm>

// AWCException - Base

AWC::Exception::Exception(std::string msg) : msg_{msg}
{

}

const char* AWC::Exception::what() const noexcept
{
    return msg_.c_str();
}

// AWC::InvalidIndexException

const std::string AWC::InvalidIndexException::INDEX_TOKEN = "%i";
const std::string AWC::InvalidIndexException::PREFIX_TOKEN = "%p";

AWC::InvalidIndexException::InvalidIndexException(std::string formattedMsg, std::string prefix, Vector2 pos) 
    : index{pos}, AWC::Exception{GetMessage(formattedMsg, prefix, pos)}
{
    
}

AWC::InvalidIndexException::InvalidIndexException(std::string formattedMsg, std::string prefix, int x, int y) 
    : index{Vector2{x, y}}, AWC::Exception{GetMessage(formattedMsg, prefix, {x, y})}
{
    
}

AWC::InvalidIndexException::InvalidIndexException(std::string formattedMsg, Vector2 pos) 
    : index{pos}, AWC::Exception{GetMessage(formattedMsg, std::string{}, pos)}
{

}

AWC::InvalidIndexException::InvalidIndexException(std::string formattedMsg, int x, int y) 
    : AWC::InvalidIndexException(formattedMsg, Vector2{x, y})
{

}

std::string AWC::InvalidIndexException::GetMessage(std::string formattedMsg, std::string prefix, Vector2 index) const
{
    // Replace index
    formattedMsg = StringUtils::Replace(formattedMsg, INDEX_TOKEN, index.ToString());
    formattedMsg = StringUtils::Replace(formattedMsg, PREFIX_TOKEN, prefix);

    return formattedMsg;
}

// NoExistingIndex

AWC::NoExistingIndexException::NoExistingIndexException(std::string prefix, Vector2 index) 
    : AWC::InvalidIndexException("%p: Index %i did not exist", prefix, index)
{

}

AWC::NoExistingIndexException::NoExistingIndexException(std::string prefix, int x, int y) 
    : AWC::NoExistingIndexException(prefix, Vector2{x, y})
{

}

AWC::NoExistingIndexException::NoExistingIndexException(Vector2 index) 
    : AWC::InvalidIndexException("Index %i did not exist", index)
{

}

AWC::NoExistingIndexException::NoExistingIndexException(int x, int y) 
    : AWC::NoExistingIndexException(Vector2{x, y})
{

}

// Already existing

AWC::AlreadyExistingIndexException::AlreadyExistingIndexException(std::string prefix, Vector2 index)
    : AWC::InvalidIndexException("%p: Index %i already exists", prefix, index)
{

}

AWC::AlreadyExistingIndexException::AlreadyExistingIndexException(std::string prefix, int x, int y)
    : AWC::AlreadyExistingIndexException(prefix, Vector2{x, y})
{

}

AWC::AlreadyExistingIndexException::AlreadyExistingIndexException(Vector2 index) 
    : AWC::InvalidIndexException("Index %i already exists", index)
{

}

AWC::AlreadyExistingIndexException::AlreadyExistingIndexException(int x, int y)  
    : AWC::AlreadyExistingIndexException(Vector2{x, y})
{

}