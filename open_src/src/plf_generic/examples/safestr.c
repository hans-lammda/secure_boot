#include <plf/platform.h>
#include <plf/safestr.h>
#define BUF_LEN 10 
int main() 
{
  char target[BUF_LEN];
  char empty_source[BUF_LEN] = "";
  char one_char[BUF_LEN] = "O";
  char max_source[BUF_LEN] = "123456789";
  char overflow_source[] = "123456789abcdefg";
  int ret ; 
  /* Copy empty string */
  ret = NStrSCopy(target,empty_source,BUF_LEN);
  if ( ! ((strcmp(target,empty_source) == 0) && ( ret == SAFESTR_OK )) ) 
    return 1;
  /* Copy maximum source */
  ret = NStrSCopy(target,max_source,BUF_LEN);
  if ( ! ((strcmp(target,max_source) == 0) && ( ret == SAFESTR_OK ))  ) 
    return 1;
  /* Copy overflow source */
  ret = NStrSCopy(target,overflow_source,BUF_LEN);
  if ( ! ((strcmp(target,max_source) == 0) && ( ret == SAFESTR_ERROR_INSUFFICIENT_BUFFER ))  ) 
    return 1;
  /* Wrong target */
  ret = NStrSCopy(NULL,max_source,BUF_LEN);
  if (  ret != SAFESTR_ERROR_INVALID_PARAMETER  ) 
    return 1;
  /* Copy NULL */
  ret = NStrSCopy(target,NULL,BUF_LEN);
  if ( ! ((strcmp(target,"") == 0) && ( ret == SAFESTR_OK ))  ) 
    return 1;
  /* 0 as max length */
  ret = NStrSCopy(target,max_source,0);
  if ( ret != SAFESTR_ERROR_INVALID_PARAMETER   ) 
    return 1;
  /* Copy one character */
  ret = NStrSCopy(target,one_char,BUF_LEN);
  if ( ! ((strcmp(target,"O") == 0) && ( ret == SAFESTR_OK ))  ) 
    return 1;
  /* Copy one character that just fits in target */
  ret = NStrSCopy(target,one_char,2);
  if ( ! ((strcmp(target,"O") == 0) && ( ret == SAFESTR_OK ))  ) 
    return 1;
  printf("Safe string handling OK\n");
  return 0;
}
