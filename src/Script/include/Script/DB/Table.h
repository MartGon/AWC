#include <unordered_map>

namespace DB
{
    template<typename T>
    class Table
    {
    public:

        T& GetById(unsigned int id)
        {
            return umap_.at(id);
        }

        void Add(T entry)
        {
            umap_.insert({index_++, entry});
        }

    private:
        std::unordered_map<unsigned int, T> umap_;
        unsigned int index_;
    };
}