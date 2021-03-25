#pragma once

#include <Utils.h>

#include <exception>
#include <string>

namespace AWC
{
    class Exception : public std::exception
    {
    public:
        virtual ~Exception() {}
        Exception(std::string msg);

        const char* what() const noexcept override;

    protected:
        const std::string msg_;
    };

    class InvalidIndexException : public Exception
    {
    public:
        virtual ~InvalidIndexException() {};
        InvalidIndexException(std::string formattedMsg, std::string prefix, Vector2 index);
        InvalidIndexException(std::string formattedMsg, std::string prefix, int x, int y);
        InvalidIndexException(std::string formattedMsg, Vector2 index);
        InvalidIndexException(std::string formattedMsg, int x, int y);

        const Vector2 index;

    protected:

        static const std::string INDEX_TOKEN;
        static const std::string PREFIX_TOKEN;

    private:
        std::string GetMessage(std::string formatted_msg, std::string prefix, Vector2 index) const;
    };

    class NoExistingIndexException : public InvalidIndexException
    {
    public:
        NoExistingIndexException(std::string prefix, Vector2 index);
        NoExistingIndexException(std::string prefix, int x, int y);
        NoExistingIndexException(Vector2 index);
        NoExistingIndexException(int x, int y);
    };

    class AlreadyExistingIndexException : public InvalidIndexException
    {
    public:
        AlreadyExistingIndexException(std::string prefix, Vector2 index);
        AlreadyExistingIndexException(std::string prefix, int x, int y);
        AlreadyExistingIndexException(Vector2 index);
        AlreadyExistingIndexException(int x, int y);
    };
}