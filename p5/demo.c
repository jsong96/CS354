#include <stdio.h>                                                                                                             

int main() {                                                                                                                   
    double array[5][5][15];                                                                                                    
    void *root = &array[0][0][0];                                                                                             
    void *addr = &array[3][4][5];                                                                                             
    size_t off = addr - root;                                                                                              
    printf("memory offset: %ld number offset: %ld\n", off, off/sizeof(double));                                                                        
    return 0;                                                                                                              
}
