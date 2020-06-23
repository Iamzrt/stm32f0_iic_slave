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
#ifndef  _SYS_CONFIG_H
#define  _SYS_CONFIG_H


/*< Print output config. 1:Enable; 0:Disable. >*/
#define          PRINTF_ENABLE          0





#if   PRINTF_ENABLE
#define				LOG(x,...)			    printf(x,##__VA_ARGS__)
#else
#define				LOG(x,...)
#endif  /* endif PRINTF_ENABLE */


#endif    /* _SYS_CONFIG_H */

/*------------------ end of file ---------------------------------------------*/
