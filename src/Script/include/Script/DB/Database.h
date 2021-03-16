#pragma once

#include <Script/DB/Table.h>

#include <tuple>

namespace DB
{
    template <class T, class Tuple>
    struct Index;

    template <class T, class... Types>
    struct Index<T, std::tuple<T, Types...>> {
        static const std::size_t value = 0;
    };

    template <class T, class U, class... Types>
    struct Index<T, std::tuple<U, Types...>> {
        static const std::size_t value = 1 + Index<T, std::tuple<Types...>>::value;
    };

    template<class T, class... Types>
    struct Database 
    {
        using type = std::tuple<Table<T>, Table<Types>...>;
        std::tuple<Table<T>, Table<Types>...> t;
        
        /*
        Database(Table<T> r, Table<Types>... args){
            t = std::make_tuple(r,args...);
        }
        */
       
        template<class I>
        using IndexDB = Index<Table<I>, type>;

        template<class Type>
        constexpr auto& get() {
            return std::get<IndexDB<Type>::value>(t);
        }
    };
}