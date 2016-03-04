#ifndef _NO_COPYABLE_H_
#define _NO_COPYABLE_H_


class NoCopyable
{
protected:
	NoCopyable() {}
	~NoCopyable() {}
private:  // emphasize the following members are private
	NoCopyable( const NoCopyable& );
	const NoCopyable& operator=( const NoCopyable& );
};

#endif