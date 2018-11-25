#pragma once


namespace NkStream
{
	class CStream;
}


namespace NkType
{
	class CString
	{
	public:
		CString();
		virtual ~CString();
		CString(const CString& d);
		CString(const wchar_t* psz);
		CString(const char* psz);
		CString &operator =(const CString& d);
		CString &operator =(const wchar_t* psz);
		CString &operator =(const char* psz);
		CString &operator +=(const CString& d);
		CString &operator +=(const wchar_t* psz);
		CString &operator +=(const char* psz);
		bool operator==(const wchar_t* psz);
		bool operator!=(const wchar_t* psz);
		operator const wchar_t*() const;
		operator wchar_t*();

		int wscanf_s(const wchar_t* pszFormat, ...);
		int printf(const wchar_t* pszFormat, ...);

		const wchar_t* data() const;
		wchar_t* buffer(size_t cch);
		void release_buffer();
		void clear();

		//Quotes the string
		void quote();

		//trim white spaces
		void trim_right();
		void trim_left();
		void trim();

		//find: returns position if found else -1  
		long find(const wchar_t* pszWhat, int iStartPos = 0);

		//extraction
		CString right(long first);
		CString left(long last);
		CString mid(long first, long count);


		//extract file name from path
		CString file_name();

		//extract file name from path and set it to the string
		void file_name(const char* psz);

		//cch: count of characters without the terminating null character
		//cch_lenght returns -1 if null string
		long lenght() const;

		size_t byte_lenght() const;

		void format_system_message(DWORD dwMessageId
			, DWORD dwLanguageId = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT));

		void read(NkStream::CStream& stream) const;
		void write(NkStream::CStream& stream);

	private:
		wchar_t* m_psz;
		size_t m_cch_allocated_with_null;
		size_t m_cch_lenght_with_null;
	};

	inline NkStream::CStream& operator<<(NkStream::CStream& l, const CString& r)
	{
		r.read(l);
		return l;
	}
	inline NkStream::CStream& operator>>(NkStream::CStream& l, CString& r)
	{
		r.write(l);
		return l;
	}
}