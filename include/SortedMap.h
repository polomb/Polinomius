#ifndef __Sorted_Map__
#define __Sorted_Map__

#include <vector>
#include <utility>
#include <algorithm>
#include <stdexcept>

template<typename Tkey, typename Tvalue>
class SortedMap
{
	std::vector<std::pair<Tkey, Tvalue>> data;
public:
	//
	//Iterator
	class Iterator
	{
	public:
		typename std::vector<std::pair<Tkey, Tvalue>>::iterator iter;
		//
		Iterator() = default;
		Iterator(typename std::vector<std::pair<Tkey, Tvalue>>::iterator Iter) : iter(Iter) {}
		//
		std::pair<Tkey, Tvalue>& operator*() const
		{
			return *iter;
		}
		std::pair<Tkey, Tvalue>* operator->() const
		{
			return iter.operator->();
		}
		//
		Iterator& operator++()
		{
			++iter;
			return *this;
		}
		Iterator operator++(int)
		{
			Iterator temp = *this;
			++iter;
			return temp;
		}
		//
		bool operator==(const Iterator& other) const
		{
			return iter == other.iter;
		}
		bool operator!=(const Iterator& other) const
		{
			return iter != other.iter;
		}
		// Something important
		operator typename std::vector<std::pair<Tkey, Tvalue>>::iterator() const
		{
			return iter;
		}
	};
	Iterator begin()
	{
		return Iterator(data.begin());
	}
	Iterator end()
	{
		return Iterator(data.end());
	}
	//
	SortedMap() = default;
	Iterator find(const Tkey& key)
	{
		Iterator it = std::lower_bound(data.begin(), data.end(), key,\
			[](const std::pair<Tkey, Tvalue>& pair, const Tkey& key)\
			{return pair.first < key;});
		
		if (it != end() && it->first == key)
			return Iterator(it); // Ключ == найденному
		else
			return Iterator(data.end());
	}
	Tvalue& operator[](const Tkey& key)
	{
		Iterator it = find(key);
		if (it != end())
			return it->second;
		else
		{
			insert(key, Tvalue());
			return find(key)->second;
		}
	}
	Tvalue& at(const Tkey& key)
	{
		Iterator it = find(key);
		if (it != data.end())
			return it->second;
		else
			throw std::out_of_range("-_-");
	}
	Iterator insert(const Tkey& key, const Tvalue& val)
	{
		Iterator it = std::lower_bound(data.begin(), data.end(), key, \
			[](const std::pair<Tkey, Tvalue>& pair, const Tkey& key)\
		{return pair.first < key; });

		if (it != data.end() && it->first == key)
			return Iterator(data.end()); //Теперь это нам не нужно, возвращаем data.end()
		
		data.insert(it, std::make_pair(key, val));
		return find(key);
	}
	Iterator erase(const Tkey& key)
	{
		Iterator it = std::lower_bound(data.begin(), data.end(), key, \
			[](const std::pair<Tkey, Tvalue>& pair, const Tkey& key)\
		{return pair.first < key; });

		if (it != data.end() && it->first == key)
			data.erase(it);
		else
			throw std::out_of_range("-_-");

		it = std::lower_bound(data.begin(), data.end(), key, \
			[](const std::pair<Tkey, Tvalue>& pair, const Tkey& key)\
		{return pair.first < key; });

		return Iterator(it); //В std::map возвращают следующий итератор
	}
	//Extra
	size_t size() const
	{
		return data.size();
	}
	bool empty() const
	{
		return data.empty();
	}
	void clear()
	{
		data.clear();
	}
};

#endif

