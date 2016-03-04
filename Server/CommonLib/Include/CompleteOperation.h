#ifndef _COMPLETE_OPERATION_H_
#define _COMPLETE_OPERATION_H_



class CompleteOperator: public OVERLAPPED
{
public:
	CompleteOperator();
	~CompleteOperator();
	uint32 GetBytesTransferred(){return static_cast<uint32>( InternalHigh );}
	int32 nMask;
};

#endif