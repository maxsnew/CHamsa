/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   Prototypes for the Deep Shallow Suffix Sort routines
   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */ 
#ifndef __DeepShallow_Suffix_Sort_H
#define __DeepShallow_Suffix_Sort_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
void ds_ssort(unsigned char *t, int *sa, int n);
int init_ds_ssort(int adist, int bs_ratio);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*#ifndef __DeepShallow_Suffix_Sort_H*/
