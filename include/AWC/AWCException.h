#pragma once

#include <Utils/Vector2.h>

#include <exception>
#include <string>

class AWCException : public std::exception
{
public:
    virtual ~AWCException() {};
    AWCException(std::string msg);

    const char* what() const noexcept override;

protected:
    const std::string msg_;
};

class AWCInvalidIndexException : public AWCException
{
public:
    virtual ~AWCInvalidIndexException() {};
    AWCInvalidIndexException(std::string formatted_msg, Vector2 index);
    AWCInvalidIndexException(std::string formatted_msg, int x, int y);

    const Vector2 index;

private:
    std::string GetMessage(std::string formatted_msg) const;

    const std::string FORMAT_TOKEN = "%p";
};

class AWCNoExistingIndexException : public AWCInvalidIndexException
{
public:
    AWCNoExistingIndexException(Vector2 index);
    AWCNoExistingIndexException(int x, int y);
};

class AWCAlreadyExistingIndexException : public AWCInvalidIndexException
{
public:
    AWCAlreadyExistingIndexException(Vector2 index);
    AWCAlreadyExistingIndexException(int x, int y);
};