#ifndef _LRUCACHE_H_
#define _LRUCACHE_H_
#include <sstream>
#include <list>
#include <unordered_map>
#include <stdexcept>


template <typename Key, typename Value>
class LRUCache
{
public:
        typedef typename std::pair<Key, Value> Entry;
        typedef typename std::list<Entry> EntryList;
        typedef typename EntryList::iterator EntryListIterator;
        typedef typename std::unordered_map<Key, EntryListIterator> HashTable;
        typedef typename HashTable::iterator HashTableIterator;
        LRUCache(std::size_t c) :
                capacity(c){
        }

        virtual ~LRUCache() {}

        void put(const Key& k, const Value& v) {
                remove(k);
                entryList.push_front(Entry(k, v));
                hashTable[k] = entryList.begin();
                if (hashTable.size() > capacity) {
                        EntryListIterator last = entryList.end();
                        --last;
                        hashTable.erase(last->first);
                        entryList.pop_back();
                }
        }

        const Value& get(const Key& k) {
                HashTableIterator it = hashTable.find(k);
                if (it == hashTable.end()) {
                        std::ostringstream oss;
                        oss << "Key not found in LRU cache\n";
                        throw std::range_error(oss.str());
                } else {
                        entryList.splice(entryList.begin(), entryList, it->second);
                        return it->second->second;
                }
        }

        void remove(const Key& k) {
                HashTableIterator it = hashTable.find(k);
                if (it != hashTable.end()) {
                        entryList.erase(it->second);
                        hashTable.erase(it);
                }
        }

        bool exists(const Key& k) const {
                return hashTable.find(k) != hashTable.end();
        }

        std::size_t maxSize() const {
                return capacity;
        }

        std::size_t size() const {
                return hashTable.size();
        }

private:
        const std::size_t capacity;
        EntryList entryList;
        HashTable hashTable;
};
#endif // _LRUCACHE_H_
