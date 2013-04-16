#ifndef _THASHTABLE_H_
#define _THASHTABLE_H_

template<class TKEY, class TVALUE>
class THashItem {
private:
	const TKEY key;
	const TVALUE value;
	THashItem* next;
public:
	THashItem(const TKEY& rtKey, const TVALUE& rtValue): key(rtKey), value(rtValue) {
		next = 0;
	}
	~THashItem() {
	}
	void SetKey(const TKEY& rtKey) {
		key = rtKey;
	}
	void SetValue(const TVALUE& rtValue) {
		value = rtValue;
	}
	void SetNext(THashItem* pkNext) {
		next = pkNext;
	}
	const TKEY& GetKey() const  {
		return key;
	}
	const TVALUE& GetValue() const  {
		return value;
	}
	THashItem* GetNext()  {
		return next;
	}
};

template<class TKEY, class TVALUE>
class THashTable {
private:
	THashItem<TKEY, TVALUE>** hashItems;
	THashItem<TKEY, TVALUE>* listIndex;
	int itemIndex;
	int size;
public:
	int (*UserHashFunction)(const TKEY&);
	THashTable(int iSize, int (*pfUserHashFunction)(const TKEY&)) {
		size = iSize;
		UserHashFunction = pfUserHashFunction;
		hashItems = (THashItem<TKEY, TVALUE>**)malloc(sizeof(THashItem<TKEY, TVALUE>*)*size);
		int i = 0;
		while (i<size) {
			hashItems[i]= 0;
			i++;
		}
	}
	THashTable(THashTable& rtHashTable) {
		size = rtHashTable.size;
		UserHashFunction = rtHashTable.UserHashFunction;
		hashItems = (THashItem<TKEY, TVALUE>**)malloc(sizeof(THashItem<TKEY, TVALUE>*)*size);
		int i = 0;
		while (i<size) {
			hashItems[i]= 0;
			i++;
		}
		TKEY* key;
		TVALUE* value;
		value = rtHashTable.GetFirst(key);
		while (key) {
			Insert(key, *(value));
			value = rtHashTable.GetNext(key);
		}
	}
	~THashTable() {	
		free(hashItems);
	}
	bool Insert(const TKEY& rtKey, const TVALUE& rtValue) {
		THashItem<TKEY, TVALUE>* item = 0;
		int key = Hash(rtKey);
		item = hashItems[key];
		if (!item) {
			hashItems[key] = new THashItem<TKEY, TVALUE>(rtKey, rtValue);
			return true;
		} else {
			while (item->GetKey() != rtKey && item->GetNext()) {
				item = item->GetNext();
			}
			if (item->GetKey() != rtKey) {
				item->SetNext(new THashItem<TKEY, TVALUE>(rtKey, rtValue));
				return true;
			}
			return false;
		}
	}

	bool Remove(const TKEY& rtKey) {
		THashItem<TKEY, TVALUE>* item = 0;
		int key = Hash(rtKey);
		item = hashItems[key];
		if (!item) {
			return false;
		} else {
			THashItem<TKEY, TVALUE>* back = 0;
			while (item->GetKey() != rtKey && item->GetNext()) {
				back = item;
				item = item->GetNext();
			}
			if (item->GetKey() == rtKey) {
				if (back) {
					back->SetNext(item->GetNext());
				} else {
					hashItems[key] = item->GetNext();
				}
				delete item;
				return true;
			}
			return false;
		}
	}

	const TVALUE Find(const TKEY& rtKey) const {
		THashItem<TKEY, TVALUE>* item = 0;
		int key = Hash(rtKey);
		item = hashItems[key];
		if (!item) {
			return 0;
		} else {
			bool found = item->GetKey() == rtKey;
			while (!found && item->GetNext()) {
				item = item->GetNext();
				found = item->GetKey() == rtKey;
			}
			return found ? item->GetValue() : 0;
		}
	}

	const TVALUE GetFirst(TKEY* ptKey) {
		itemIndex = 0;
		listIndex = 0;
		while (!hashItems[itemIndex] && itemIndex < size) {
			itemIndex++;
		}
		if (hashItems[itemIndex] && itemIndex < size) {
			listIndex = hashItems[itemIndex];
			if (ptKey) {
				*ptKey = listIndex->GetKey();
			}
			return listIndex->GetValue();
		} 
		*ptKey = 0;
		return 0;
	}

	const TVALUE GetNext(TKEY* ptKey) {
		if (listIndex) {
			if (listIndex->GetNext()) {
				listIndex = listIndex->GetNext();
				if (ptKey) {
					*ptKey = listIndex->GetKey();
				}
				return listIndex->GetValue();
			}
			itemIndex++;
			while (!hashItems[itemIndex] && itemIndex < size) {
				itemIndex++;
			}
			if (hashItems[itemIndex] && itemIndex < size) {
				listIndex = hashItems[itemIndex];
				if (ptKey) {
					*ptKey = listIndex->GetKey();
				}
				return listIndex->GetValue();
			} 
		}
		ptKey = 0;
		return 0;
	}

private:
	int DefaultHashFunction(const TKEY& key) const {
		return key % size;
	}
	int Hash(const TKEY& rtKey) const {
		if (UserHashFunction) {
			return UserHashFunction(rtKey);
		} else {
			return DefaultHashFunction(rtKey);
		}
	}
};

#endif