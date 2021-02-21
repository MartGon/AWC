#pragma once

#include <Utils.h>

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
    AWCInvalidIndexException(std::string formattedMsg, std::string prefix, Vector2 index);
    AWCInvalidIndexException(std::string formattedMsg, std::string prefix, int x, int y);
    AWCInvalidIndexException(std::string formattedMsg, Vector2 index);
    AWCInvalidIndexException(std::string formattedMsg, int x, int y);

    const Vector2 index;

protected:

    static const std::string INDEX_TOKEN;
    static const std::string PREFIX_TOKEN;

private:
    std::string GetMessage(std::string formatted_msg, std::string prefix, Vector2 index) const;
};

class AWCNoExistingIndexException : public AWCInvalidIndexException
{
public:
    AWCNoExistingIndexException(std::string prefix, Vector2 index);
    AWCNoExistingIndexException(std::string prefix, int x, int y);
    AWCNoExistingIndexException(Vector2 index);
    AWCNoExistingIndexException(int x, int y);
};

class AWCAlreadyExistingIndexException : public AWCInvalidIndexException
{
public:
    AWCAlreadyExistingIndexException(std::string prefix, Vector2 index);
    AWCAlreadyExistingIndexException(std::string prefix, int x, int y);
    AWCAlreadyExistingIndexException(Vector2 index);
    AWCAlreadyExistingIndexException(int x, int y);
};