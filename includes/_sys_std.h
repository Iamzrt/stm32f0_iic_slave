/**
  ******************************************************************************
  * @file    
  * @author  
  * @version 
  * @date   
  * @brief   
  ******************************************************************************  
  * 
  * 
  ******************************************************************************
  */
#ifndef  _SYS_STD_H
#define  _SYS_STD_H


/* C++֧�� */
#ifdef __cplusplus
extern "C" {
#endif
 


/* ��ͬIDE��Ҫ�޸��������Ͷ��� */
/*!< Signed integer types  */
typedef         signed char             s8_t;
typedef         signed short int        s16_t;
typedef         signed int              s32_t;

/*!< Unsigned integer types  */
typedef         unsigned char           u8_t;
typedef         unsigned short int      u16_t;
typedef         unsigned int            u32_t;



#define   XDATA
#define   IDATA
#define   BDATA
#define   DATA
#define   CODE


/* �Ѻ������Ϊһ���ַ��� */
#define     _STRING(x)                  #x
#define     STRING(x)                   _STRING(x)

/* �Ѻ����������һ�� */
#define     _CONNECT_MACRO(x,y)         x##y
#define     CONNECT_MACRO(x,y)          _CONNECT_MACRO(x,y)


/* ״̬���� */
typedef  enum
{
  OPERATE_OK  = 0,
  OPERATE_BUSY,
  OPERATE_TIMEOUT,
  OPERATE_FAIL,
}SysStatusType;


/* ������� */
typedef   enum
{
  USER_ERROR_0 = 0,
	USER_ERROR_1,
	USER_ERROR_2,
	USER_ERROR_3,
	USER_ERROR_4,
	USER_ERROR_5,
	USER_ERROR_6,
	USER_ERROR_7,
	USER_ERROR_8,
	USER_ERROR_9,
	USER_ERROR_10,
}UserErrorCode;


#ifndef  _IN_LINE_
  #define  _IN_LINE_   inline
#endif

#ifndef   BOOL
  #define   BOOL   unsigned char
#endif

#ifndef   NULL
  #define   NULL     (void*)0
#endif

#ifndef   FALSE
	#define   FALSE     0
#endif
	
#ifndef   TRUE
	#define   TRUE      1
#endif

/* ���麯���ؼ��� */
#ifndef   VIRTUAL
  #define   VIRTUAL
#endif


#ifndef  FRIEND
	#define  FRIEND   extern 
#endif
		
#ifndef  _PACKED
	#define  _PACKED  __packed
#endif
	
#ifndef  _WEAK
	#define  _WEAK   __weak
#endif
	
#ifndef  PROTECTED
	#define  PROTECTED
#endif
	
	
#ifndef  PUBLIC
	#define  PUBLIC
#endif
	
	
#ifndef  ABSTRACT
	#define   ABSTRACT   /* pure virtual function. */
#endif
	
	
#ifndef  	PACKED
	#define    PACKED   __packed
#endif


/* �ⲿ���������ؼ��� */
#ifndef   EXTERN_FUNC
  #define   EXTERN_FUNC         extern
#endif

#ifndef   INTERRUPT_FUNC
	#define   INTERRUPT_FUNC
#endif
	
	


/* ����״̬��FSM(finite state machine)ʵ�� */
typedef   unsigned char    FSMState;
typedef   FSMState(*Procedure)(void * arg);



#ifdef __cplusplus
}
#endif

#endif    /* _SYS_STD_H */

/*------------------ end of file ---------------------------------------------*/
