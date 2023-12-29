#ifndef H_INCLUDED_RING_BUFFER
#define H_INCLUDED_RING_BUFFER

template<typename T>
class RingBuffer
{
public:
	RingBuffer(int sz);

	void Add(T toAdd);
	T* GetAtOffsetFromHead(int offset) const;
	int GetCurSize() const;

	~RingBuffer();
protected:
	T* m_data;
	int m_totSz;
	int m_curSz = 0;
	int m_headIdx = -1;
};

template<typename T>
inline RingBuffer<T>::RingBuffer(int sz)
	: m_totSz(sz)
{
	//m_data = (Vector2 *) MemAlloc(sizeof(Vector2) * sz);
	m_data = new T[sz];
}

template<typename T>
inline void RingBuffer<T>::Add(T toAdd)
{
	if (m_curSz < m_totSz)
		m_curSz++;

	if (m_headIdx != m_totSz - 1)
	{
		m_headIdx++;
	}
	else
	{
		m_headIdx = 0;
	}

	m_data[m_headIdx] = toAdd;
}

template<typename T>
inline T* RingBuffer<T>::GetAtOffsetFromHead(int offset) const
{
	if (offset >= m_curSz || offset >= m_totSz)
	{
		// error: don't have that many elements
		return nullptr;
	}

	int idx = m_headIdx - offset;
	if (idx < 0)
	{
		idx = m_curSz + idx;
	}

	return &m_data[idx];
}

template<typename T>
inline int RingBuffer<T>::GetCurSize() const
{
	return m_curSz;
}

template<typename T>
inline RingBuffer<T>::~RingBuffer()
{
	delete[] m_data;
}

#endif