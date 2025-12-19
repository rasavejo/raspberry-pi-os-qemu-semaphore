#ifndef _USER_FUT_H
#define _USER_FUT_H

 extern unsigned long fut_new(unsigned long page,unsigned int count);
 extern void fut_delete(unsigned long page,unsigned long fut);
 extern void fut_p(unsigned long page,unsigned long fut);
 extern void fut_v(unsigned long page,unsigned long fut);


#endif /* _USER_FUT_H */