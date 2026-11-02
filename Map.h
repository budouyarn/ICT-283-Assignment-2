#ifndef MAP_H
#define MAP_H

#include <map>
#include "Vector.h"

/**
 * A Map associates a KEY with a VALUE: give it a key and it gives back the value stored for that key.
 * It wraps std::map and shows only what the programme needs, so nothing std::map-specific
 * (iterators, std::pair, operator[]) appears in the public interface. Client code therefore would not
 * change if the std::map inside were later replaced by a Vector of key/value entries
 * (TestMap.cpp runs the same tests against both).
 *
 * Duplicate keys are NOT allowed: a key has at most one value. Insert refuses a key that is already
 * there. (In this programme a year has exactly one tree of readings, so a second value for the
 * same year would be a mistake that should be noticed, not silently accepted.)
 *
 * K must support operator<. V must be copyable (Insert stores a copy).
 *
 * The Rule of Three needs no code here: std::map manages its own memory, so the compiler's
 * copy, assignment and destructor already give every Map its own independent copy of the data.
 *
 * @tparam K the type of the key
 * @tparam V the type of the value
 *
 * @class Map
 * @author  Lim Wen Xin Sherine
 * @version 1.0
 * @date    01/11/2026
 */
template <class K, class V>
class Map
{
    public:
        /** @brief Default constructor. Creates an empty Map. */
        Map();

        /**
         * @brief Associates a key with a value.
         * @param key: the key to add
         * @param value: the value to store for that key (a copy is kept)
         * @return true if added, false if the key was already in the Map (nothing changes)
         */
        bool Insert( const K & key, const V & value );

        /**
         * @brief Finds the value stored for a key. The value can be read and changed through the pointer.
         * @param key: the key to look for
         * @return a pointer to the value, or nullptr if the key is not in the Map.
         *         The pointer stays valid until that key is deleted or the Map is destroyed.
         */
        V * Find( const K & key );

        /** @brief Read-only version of Find, for a const Map. */
        const V * Find( const K & key ) const;

        /**
         * @brief Removes a key and its value.
         * @param key: the key to remove
         * @return true if it was removed, false if the key was not there
         */
        bool Delete( const K & key );

        /** @brief How many keys the Map holds.
         *  @return the number of keys */
        int Size() const;

        /**
         * @brief Lists every key, so the client can visit every value with Find.
         * @return the keys in ascending order
         */
        Vector<K> Keys() const;

    private:
        std::map<K, V> m_map;       /**< the key/value pairs, kept in key order by std::map */
};

template <class K, class V>
Map<K, V>::Map() : m_map()
{
}

template <class K, class V>
bool Map<K, V>::Insert( const K & key, const V & value )
{
    return m_map.insert( std::pair<const K, V>( key, value ) ).second;   // .second is false if the key was already there
}

template <class K, class V>
V * Map<K, V>::Find( const K & key )
{
    typename std::map<K, V>::iterator found = m_map.find( key );
    if (found == m_map.end())
        return nullptr;
    return &found->second;
}

template <class K, class V>
const V * Map<K, V>::Find( const K & key ) const
{
    typename std::map<K, V>::const_iterator found = m_map.find( key );
    if (found == m_map.end())
        return nullptr;
    return &found->second;
}

template <class K, class V>
bool Map<K, V>::Delete( const K & key )
{
    return m_map.erase( key ) == 1;
}

template <class K, class V>
int Map<K, V>::Size() const
{
    return static_cast<int>( m_map.size() );
}

template <class K, class V>
Vector<K> Map<K, V>::Keys() const
{
    Vector<K> keys;
    for (typename std::map<K, V>::const_iterator it = m_map.begin(); it != m_map.end(); ++it)
        keys.Insert( keys.Size(), it->first );
    return keys;
}

#endif // MAP_H
