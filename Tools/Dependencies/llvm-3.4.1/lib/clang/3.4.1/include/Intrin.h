/* ===-------- Intrin.h ---------------------------------------------------===
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 *===-----------------------------------------------------------------------===
 */

/* Only include this if we're compiling for the windows platform. */
#ifndef _MSC_VER
#include_next <Intrin.h>
#else

#ifndef __INTRIN_H
#define __INTRIN_H

/* First include the standard intrinsics. */
#include <x86intrin.h>

#ifdef __cplusplus
extern "C" {
#endif

/* And the random ones that aren't in those files. */
__m64 _m_from_float(float);
__m64 _m_from_int(int _l);
void _m_prefetch(void *);
float _m_to_float(__m64);
int _m_to_int(__m64 _M);

/* Other assorted instruction intrinsics. */
void __addfsbyte(unsigned long, unsigned char);
void __addfsdword(unsigned long, unsigned long);
void __addfsword(unsigned long, unsigned short);
void __code_seg(const char *);
void __cpuid(int[4], int);
void __cpuidex(int[4], int, int);
void __debugbreak(void);
__int64 __emul(int, int);
unsigned __int64 __emulu(unsigned int, unsigned int);
void __cdecl __fastfail(unsigned int);
unsigned int __getcallerseflags(void);
void __halt(void);
unsigned char __inbyte(unsigned short);
void __inbytestring(unsigned short, unsigned char *, unsigned long);
void __incfsbyte(unsigned long);
void __incfsdword(unsigned long);
void __incfsword(unsigned long);
unsigned long __indword(unsigned short);
void __indwordstring(unsigned short, unsigned long *, unsigned long);
void __int2c(void);
void __invlpg(void *);
unsigned short __inword(unsigned short);
void __inwordstring(unsigned short, unsigned short *, unsigned long);
void __lidt(void *);
unsigned __int64 __ll_lshift(unsigned __int64, int);
__int64 __ll_rshift(__int64, int);
void __llwpcb(void *);
unsigned char __lwpins32(unsigned int, unsigned int, unsigned int);
void __lwpval32(unsigned int, unsigned int, unsigned int);
unsigned int __lzcnt(unsigned int);
unsigned short __lzcnt16(unsigned short);
void __movsb(unsigned char *, unsigned char const *, size_t);
void __movsd(unsigned long *, unsigned long const *, size_t);
void __movsw(unsigned short *, unsigned short const *, size_t);
void __nop(void);
void __nvreg_restore_fence(void);
void __nvreg_save_fence(void);
void __outbyte(unsigned short, unsigned char);
void __outbytestring(unsigned short, unsigned char *, unsigned long);
void __outdword(unsigned short, unsigned long);
void __outdwordstring(unsigned short, unsigned long *, unsigned long);
void __outword(unsigned short, unsigned short);
void __outwordstring(unsigned short, unsigned short *, unsigned long);
static __inline__
unsigned int __popcnt(unsigned int);
static __inline__
unsigned short __popcnt16(unsigned short);
unsigned __int64 __rdtsc(void);
unsigned __int64 __rdtscp(unsigned int *);
unsigned long __readcr0(void);
unsigned long __readcr2(void);
unsigned long __readcr3(void);
unsigned long __readcr5(void);
unsigned long __readcr8(void);
unsigned int __readdr(unsigned int);
unsigned int __readeflags(void);
unsigned char __readfsbyte(unsigned long);
unsigned long __readfsdword(unsigned long);
unsigned __int64 __readfsqword(unsigned long);
unsigned short __readfsword(unsigned long);
unsigned __int64 __readmsr(unsigned long);
unsigned __int64 __readpmc(unsigned long);
unsigned long __segmentlimit(unsigned long);
void __sidt(void *);
void *__slwpcb(void);
void __stosb(unsigned char *, unsigned char, size_t);
void __stosd(unsigned long *, unsigned long, size_t);
void __stosw(unsigned short *, unsigned short, size_t);
void __svm_clgi(void);
void __svm_invlpga(void *, int);
void __svm_skinit(int);
void __svm_stgi(void);
void __svm_vmload(size_t);
void __svm_vmrun(size_t);
void __svm_vmsave(size_t);
void __ud2(void);
unsigned __int64 __ull_rshift(unsigned __int64, int);
void __vmx_off(void);
void __vmx_vmptrst(unsigned __int64 *);
void __wbinvd(void);
void __writecr0(unsigned int);
void __writecr3(unsigned int);
void __writecr4(unsigned int);
void __writecr8(unsigned int);
void __writedr(unsigned int, unsigned int);
void __writeeflags(unsigned int);
void __writefsbyte(unsigned long, unsigned char);
void __writefsdword(unsigned long, unsigned long);
void __writefsqword(unsigned long, unsigned __int64);
void __writefsword(unsigned long, unsigned short);
void __writemsr(unsigned long, unsigned __int64);
static __inline__
void *_AddressOfReturnAddress(void);
unsigned int _andn_u32(unsigned int, unsigned int);
unsigned int _bextr_u32(unsigned int, unsigned int, unsigned int);
unsigned int _bextr_u32(unsigned int, unsigned int, unsigned int);
unsigned int _bextri_u32(unsigned int, unsigned int);
static __inline__
unsigned char _BitScanForward(unsigned long *_Index, unsigned long _Mask);
static __inline__
unsigned char _BitScanReverse(unsigned long *_Index, unsigned long _Mask);
static __inline__
unsigned char _bittest(long const *, long);
static __inline__
unsigned char _bittestandcomplement(long *, long);
static __inline__
unsigned char _bittestandreset(long *, long);
static __inline__
unsigned char _bittestandset(long *, long);
unsigned int _blcfill_u32(unsigned int);
unsigned int _blci_u32(unsigned int);
unsigned int _blcic_u32(unsigned int);
unsigned int _blcmsk_u32(unsigned int);
unsigned int _blcs_u32(unsigned int);
unsigned int _blsfill_u32(unsigned int);
unsigned int _blsi_u32(unsigned int);
unsigned int _blsic_u32(unsigned int);
unsigned int _blsmsk_u32(unsigned int);
unsigned int _blsmsk_u32(unsigned int);
unsigned int _blsr_u32(unsigned int);
unsigned int _blsr_u32(unsigned int);
unsigned __int64 __cdecl _byteswap_uint64(unsigned __int64);
unsigned long __cdecl _byteswap_ulong(unsigned long);
unsigned short __cdecl _byteswap_ushort(unsigned short);
unsigned _bzhi_u32(unsigned int, unsigned int);
void __cdecl _disable(void);
void __cdecl _enable(void);
void __cdecl _fxrstor(void const *);
void __cdecl _fxsave(void *);
long _InterlockedAddLargeStatistic(__int64 volatile *_Addend, long _Value);
static __inline__
long _InterlockedAnd(long volatile *_Value, long _Mask);
static __inline__
short _InterlockedAnd16(short volatile *_Value, short _Mask);
static __inline__
char _InterlockedAnd8(char volatile *_Value, char _Mask);
unsigned char _interlockedbittestandreset(long volatile *, long);
unsigned char _interlockedbittestandset(long volatile *, long);
static __inline__
long __cdecl _InterlockedCompareExchange(long volatile *_Destination,
                                         long _Exchange, long _Comparand);
long _InterlockedCompareExchange_HLEAcquire(long volatile *, long, long);
long _InterlockedCompareExchange_HLERelease(long volatile *, long, long);
static __inline__
short _InterlockedCompareExchange16(short volatile *_Destination,
                                    short _Exchange, short _Comparand);
static __inline__
__int64 _InterlockedCompareExchange64(__int64 volatile *_Destination,
                                      __int64 _Exchange, __int64 _Comparand);
__int64 _InterlockedcompareExchange64_HLEAcquire(__int64 volatile *, __int64,
                                                 __int64);
__int64 _InterlockedCompareExchange64_HLERelease(__int64 volatile *, __int64,
                                                 __int64);
static __inline__
char _InterlockedCompareExchange8(char volatile *_Destination, char _Exchange,
                                  char _Comparand);
void *_InterlockedCompareExchangePointer_HLEAcquire(void *volatile *, void *,
                                                    void *);
void *_InterlockedCompareExchangePointer_HLERelease(void *volatile *, void *,
                                                    void *);
static __inline__
long __cdecl _InterlockedDecrement(long volatile *_Addend);
static __inline__
short _InterlockedDecrement16(short volatile *_Addend);
static __inline__
long __cdecl _InterlockedExchange(long volatile *_Target, long _Value);
static __inline__
short _InterlockedExchange16(short volatile *_Target, short _Value);
static __inline__
char _InterlockedExchange8(char volatile *_Target, char _Value);
static __inline__
long __cdecl _InterlockedExchangeAdd(long volatile *_Addend, long _Value);
long _InterlockedExchangeAdd_HLEAcquire(long volatile *, long);
long _InterlockedExchangeAdd_HLERelease(long volatile *, long);
static __inline__
char _InterlockedExchangeAdd8(char volatile *_Addend, char _Value);
static __inline__
long __cdecl _InterlockedIncrement(long volatile *_Addend);
static __inline__
short _InterlockedIncrement16(short volatile *_Addend);
static __inline__
long _InterlockedOr(long volatile *_Value, long _Mask);
static __inline__
short _InterlockedOr16(short volatile *_Value, short _Mask);
static __inline__
char _InterlockedOr8(char volatile *_Value, char _Mask);
static __inline__
long _InterlockedXor(long volatile *_Value, long _Mask);
static __inline__
short _InterlockedXor16(short volatile *_Value, short _Mask);
static __inline__
char _InterlockedXor8(char volatile *_Value, char _Mask);
void __cdecl _invpcid(unsigned int, void *);
static __inline__
unsigned long __cdecl _lrotl(unsigned long, int);
static __inline__
unsigned long __cdecl _lrotr(unsigned long, int);
static __inline__
unsigned int _lzcnt_u32(unsigned int);
static __inline__
void _ReadBarrier(void);
static __inline__
void _ReadWriteBarrier(void);
static __inline__
void *_ReturnAddress(void);
unsigned int _rorx_u32(unsigned int, const unsigned int);
int __cdecl _rdrand16_step(unsigned short *);
int __cdecl _rdrand32_step(unsigned int *);
static __inline__
unsigned int __cdecl _rotl(unsigned int _Value, int _Shift);
static __inline__
unsigned short _rotl16(unsigned short _Value, unsigned char _Shift);
static __inline__
unsigned __int64 __cdecl _rotl64(unsigned __int64 _Value, int _Shift);
static __inline__
unsigned char _rotl8(unsigned char _Value, unsigned char _Shift);
static __inline__
unsigned int __cdecl _rotr(unsigned int _Value, int _Shift);
static __inline__
unsigned short _rotr16(unsigned short _Value, unsigned char _Shift);
static __inline__
unsigned __int64 __cdecl _rotr64(unsigned __int64 _Value, int _Shift);
static __inline__
unsigned char _rotr8(unsigned char _Value, unsigned char _Shift);
int _sarx_i32(int, unsigned int);

/* FIXME: Need definition for jmp_buf.
   int __cdecl _setjmp(jmp_buf); */

unsigned int _shlx_u32(unsigned int, unsigned int);
unsigned int _shrx_u32(unsigned int, unsigned int);
void _Store_HLERelease(long volatile *, long);
void _Store64_HLERelease(__int64 volatile *, __int64);
void _StorePointer_HLERelease(void *volatile *, void *);
unsigned int _t1mskc_u32(unsigned int);
unsigned int _tzcnt_u32(unsigned int);
unsigned int _tzcnt_u32(unsigned int);
unsigned int _tzmsk_u32(unsigned int);
static __inline__
void _WriteBarrier(void);
void _xabort(const unsigned int imm);
unsigned __int32 xbegin(void);
void _xend(void);
unsigned __int64 __cdecl _xgetbv(unsigned int);
void __cdecl _xrstor(void const *, unsigned __int64);
void __cdecl _xsave(void *, unsigned __int64);
void __cdecl _xsaveopt(void *, unsigned __int64);
void __cdecl _xsetbv(unsigned int, unsigned __int64);
unsigned char _xtest(void);

/* These additional intrinsics are turned on in x64/amd64/x86_64 mode. */
#ifdef __x86_64__
void __addgsbyte(unsigned long, unsigned char);
void __addgsdword(unsigned long, unsigned long);
void __addgsqword(unsigned long, unsigned __int64);
void __addgsword(unsigned long, unsigned short);
void __faststorefence(void);
void __incgsbyte(unsigned long);
void __incgsdword(unsigned long);
void __incgsqword(unsigned long);
void __incgsword(unsigned long);
unsigned __int64 __popcnt64(unsigned __int64);
unsigned __int64 __shiftleft128(unsigned __int64 _LowPart,
                                unsigned __int64 _HighPart,
                                unsigned char _Shift);
unsigned __int64 __shiftright128(unsigned __int64 _LowPart,
                                 unsigned __int64 _HighPart,
                                 unsigned char _Shift);
void __stosq(unsigned __int64 *, unsigned __int64, size_t);
unsigned __int64 _andn_u64(unsigned __int64, unsigned __int64);
unsigned __int64 _bextr_u64(unsigned __int64, unsigned int, unsigned int);
unsigned __int64 _bextri_u64(unsigned __int64, unsigned int);
static __inline__
unsigned char _BitScanForward64(unsigned long *_Index, unsigned __int64 _Mask);
static __inline__
unsigned char _BitScanReverse64(unsigned long *_Index, unsigned __int64 _Mask);
static __inline__
unsigned char _bittest64(__int64 const *, __int64);
static __inline__
unsigned char _bittestandcomplement64(__int64 *, __int64);
static __inline__
unsigned char _bittestandreset64(__int64 *, __int64);
static __inline__
unsigned char _bittestandset64(__int64 *, __int64);
unsigned __int64 _blcfill_u64(unsigned __int64);
unsigned __int64 _blci_u64(unsigned __int64);
unsigned __int64 _blcic_u64(unsigned __int64);
unsigned __int64 _blcmsk_u64(unsigned __int64);
unsigned __int64 _blcs_u64(unsigned __int64);
unsigned __int64 _blsfill_u64(unsigned __int64);
unsigned __int64 _blsi_u64(unsigned __int64);
unsigned __int64 _blsic_u64(unsigned __int64);
unsigned __int64 _blmsk_u64(unsigned __int64);
unsigned __int64 _blsr_u64(unsigned __int64);
unsigned __int64 __cdecl _byteswap_uint64(unsigned __int64);
unsigned __int64 _bzhi_u64(unsigned __int64, unsigned int);
void __cdecl _fxrstor64(void const *);
void __cdecl _fxsave64(void *);
long _InterlockedAnd_np(long volatile *_Value, long _Mask);
short _InterlockedAnd16_np(short volatile *_Value, short _Mask);
__int64 _InterlockedAnd64_np(__int64 volatile *_Value, __int64 _Mask);
char _InterlockedAnd8_np(char volatile *_Value, char _Mask);
unsigned char _interlockedbittestandreset64(__int64 volatile *, __int64);
unsigned char _interlockedbittestandset64(__int64 volatile *, __int64);
long _InterlockedCompareExchange_np(long volatile *_Destination, long _Exchange,
                                    long _Comparand);
unsigned char _InterlockedCompareExchange128(__int64 volatile *_Destination,
                                             __int64 _ExchangeHigh,
                                             __int64 _ExchangeLow,
                                             __int64 *_CompareandResult);
unsigned char _InterlockedCompareExchange128_np(__int64 volatile *_Destination,
                                                __int64 _ExchangeHigh,
                                                __int64 _ExchangeLow,
                                                __int64 *_ComparandResult);
short _InterlockedCompareExchange16_np(short volatile *_Destination,
                                       short _Exchange, short _Comparand);
__int64 _InterlockedCompareExchange64_np(__int64 volatile *_Destination,
                                         __int64 _Exchange, __int64 _Comparand);
void *_InterlockedCompareExchangePointer_np(void *volatile *_Destination,
                                            void *_Exchange, void *_Comparand);
long _InterlockedOr_np(long volatile *_Value, long _Mask);
short _InterlockedOr16_np(short volatile *_Value, short _Mask);
__int64 _InterlockedOr64_np(__int64 volatile *_Value, __int64 _Mask);
char _InterlockedOr8_np(char volatile *_Value, char _Mask);
long _InterlockedXor_np(long volatile *_Value, long _Mask);
short _InterlockedXor16_np(short volatile *_Value, short _Mask);
__int64 _InterlockedXor64_np(__int64 volatile *_Value, __int64 _Mask);
char _InterlockedXor8_np(char volatile *_Value, char _Mask);
unsigned __int64 _lzcnt_u64(unsigned __int64);
__int64 _mul128(__int64 _Multiplier, __int64 _Multiplicand,
                __int64 *_HighProduct);
unsigned int __cdecl _readfsbase_u32(void);
unsigned __int64 __cdecl _readfsbase_u64(void);
unsigned int __cdecl _readgsbase_u32(void);
unsigned __int64 __cdecl _readgsbase_u64(void);
unsigned __int64 _rorx_u64(unsigned __int64, const unsigned int);
unsigned __int64 _tzcnt_u64(unsigned __int64);
unsigned __int64 _tzmsk_u64(unsigned __int64);
unsigned __int64 _umul128(unsigned __int64 _Multiplier,
                          unsigned __int64 _Multiplicand,
                          unsigned __int64 *_HighProduct);
void __cdecl _writefsbase_u32(unsigned int);
void _cdecl _writefsbase_u64(unsigned __int64);
void __cdecl _writegsbase_u32(unsigned int);
void __cdecl _writegsbase_u64(unsigned __int64);
void __cdecl _xrstor64(void const *, unsigned __int64);
void __cdecl _xsave64(void *, unsigned __int64);
void __cdecl _xsaveopt64(void *, unsigned __int64);

#endif /* __x86_64__ */

/*----------------------------------------------------------------------------*\
|* Bit Twiddling
\*----------------------------------------------------------------------------*/
static __inline__ unsigned char __attribute__((__always_inline__, __nodebug__))
_rotl8(unsigned char _Value, unsigned char _Shift) {
  _Shift &= 0x7;
  return _Shift ? (_Value << _Shift) | (_Value >> (8 - _Shift)) : _Value;
}
static __inline__ unsigned char __attribute__((__always_inline__, __nodebug__))
_rotr8(unsigned char _Value, unsigned char _Shift) {
  _Shift &= 0x7;
  return _Shift ? (_Value >> _Shift) | (_Value << (8 - _Shift)) : _Value;
}
static __inline__ unsigned short __attribute__((__always_inline__, __nodebug__))
_rotl16(unsigned short _Value, unsigned char _Shift) {
  _Shift &= 0xf;
  return _Shift ? (_Value << _Shift) | (_Value >> (16 - _Shift)) : _Value;
}
static __inline__ unsigned short __attribute__((__always_inline__, __nodebug__))
_rotr16(unsigned short _Value, unsigned char _Shift) {
  _Shift &= 0xf;
  return _Shift ? (_Value >> _Shift) | (_Value << (16 - _Shift)) : _Value;
}
static __inline__ unsigned int __attribute__((__always_inline__, __nodebug__))
_rotl(unsigned int _Value, int _Shift) {
  _Shift &= 0x1f;
  return _Shift ? (_Value << _Shift) | (_Value >> (32 - _Shift)) : _Value;
}
static __inline__ unsigned int __attribute__((__always_inline__, __nodebug__))
_rotr(unsigned int _Value, int _Shift) {
  _Shift &= 0x1f;
  return _Shift ? (_Value >> _Shift) | (_Value << (32 - _Shift)) : _Value;
}
static __inline__ unsigned long __attribute__((__always_inline__, __nodebug__))
_lrotl(unsigned long _Value, int _Shift) {
  _Shift &= 0x1f;
  return _Shift ? (_Value << _Shift) | (_Value >> (32 - _Shift)) : _Value;
}
static __inline__ unsigned long __attribute__((__always_inline__, __nodebug__))
_lrotr(unsigned long _Value, int _Shift) {
  _Shift &= 0x1f;
  return _Shift ? (_Value >> _Shift) | (_Value << (32 - _Shift)) : _Value;
}
static
__inline__ unsigned __int64 __attribute__((__always_inline__, __nodebug__))
_rotl64(unsigned __int64 _Value, int _Shift) {
  _Shift &= 0x3f;
  return _Shift ? (_Value << _Shift) | (_Value >> (64 - _Shift)) : _Value;
}
static
__inline__ unsigned __int64 __attribute__((__always_inline__, __nodebug__))
_rotr64(unsigned __int64 _Value, int _Shift) {
  _Shift &= 0x3f;
  return _Shift ? (_Value >> _Shift) | (_Value << (64 - _Shift)) : _Value;
}
/*----------------------------------------------------------------------------*\
|* Bit Counting and Testing
\*----------------------------------------------------------------------------*/
static __inline__ unsigned char __attribute__((__always_inline__, __nodebug__))
_BitScanForward(unsigned long *_Index, unsigned long _Mask) {
  if (!_Mask)
    return 0;
  *_Index = __builtin_ctzl(_Mask);
  return 1;
}
static __inline__ unsigned char __attribute__((__always_inline__, __nodebug__))
_BitScanReverse(unsigned long *_Index, unsigned long _Mask) {
  if (!_Mask)
    return 0;
  *_Index = 31 - __builtin_clzl(_Mask);
  return 1;
}
static __inline__ unsigned int __attribute__((__always_inline__, __nodebug__))
_lzcnt_u32(unsigned int a) {
  if (!a)
    return 32;
  return __builtin_clzl(a);
}
static __inline__ unsigned short __attribute__((__always_inline__, __nodebug__))
__popcnt16(unsigned short value) {
  return __builtin_popcount((int)value);
}
static __inline__ unsigned int __attribute__((__always_inline__, __nodebug__))
__popcnt(unsigned int value) {
  return __builtin_popcount(value);
}
static __inline__ unsigned char __attribute__((__always_inline__, __nodebug__))
_bittest(long const *a, long b) {
  return (*a >> b) & 1;
}
static __inline__ unsigned char __attribute__((__always_inline__, __nodebug__))
_bittestandcomplement(long *a, long b) {
  unsigned char x = (*a >> b) & 1;
  *a = *a ^ (1 << b);
  return x;
}
static __inline__ unsigned char __attribute__((__always_inline__, __nodebug__))
_bittestandreset(long *a, long b) {
  unsigned char x = (*a >> b) & 1;
  *a = *a & ~(1 << b);
  return x;
}
static __inline__ unsigned char __attribute__((__always_inline__, __nodebug__))
_bittestandset(long *a, long b) {
  unsigned char x = (*a >> b) & 1;
  *a = *a | (1 << b);
  return x;
}
#ifdef __x86_64__
static __inline__ unsigned char __attribute__((__always_inline__, __nodebug__))
_BitScanForward64(unsigned long *_Index, unsigned __int64 _Mask) {
  if (!_Mask)
    return 0;
  *_Index = __builtin_ctzll(_Mask);
  return 1;
}
static __inline__ unsigned char __attribute__((__always_inline__, __nodebug__))
_BitScanReverse64(unsigned long *_Index, unsigned __int64 _Mask) {
  if (!_Mask)
    return 0;
  *_Index = 63 - __builtin_clzll(_Mask);
  return 1;
}
static
__inline__ unsigned __int64 __attribute__((__always_inline__, __nodebug__))
_lzcnt_u64(unsigned __int64 a) {
  if (!a)
    return 64;
  return __builtin_clzll(a);
}
static __inline__
unsigned __int64 __attribute__((__always_inline__, __nodebug__))
 __popcnt64(unsigned __int64 value) {
  return __builtin_popcountll(value);
}
static __inline__ unsigned char __attribute__((__always_inline__, __nodebug__))
_bittest64(__int64 const *a, __int64 b) {
  return (*a >> b) & 1;
}
static __inline__ unsigned char __attribute__((__always_inline__, __nodebug__))
_bittestandcomplement64(__int64 *a, __int64 b) {
  unsigned char x = (*a >> b) & 1;
  *a = *a ^ (1ll << b);
  return x;
}
static __inline__ unsigned char __attribute__((__always_inline__, __nodebug__))
_bittestandreset64(__int64 *a, __int64 b) {
  unsigned char x = (*a >> b) & 1;
  *a = *a & ~(1ll << b);
  return x;
}
static __inline__ unsigned char __attribute__((__always_inline__, __nodebug__))
_bittestandset64(__int64 *a, __int64 b) {
  unsigned char x = (*a >> b) & 1;
  *a = *a | (1ll << b);
  return x;
}
#endif
/*----------------------------------------------------------------------------*\
|* Interlocked Exchange Add
\*----------------------------------------------------------------------------*/
static __inline__ char __attribute__((__always_inline__, __nodebug__))
_InterlockedExchangeAdd8(char volatile *_Addend, char _Value) {
  return __atomic_add_fetch(_Addend, _Value, 0) - _Value;
}
static __inline__ short __attribute__((__always_inline__, __nodebug__))
_InterlockedExchangeAdd16(short volatile *_Addend, short _Value) {
  return __atomic_add_fetch(_Addend, _Value, 0) - _Value;
}
static __inline__ long __attribute__((__always_inline__, __nodebug__))
_InterlockedExchangeAdd(long volatile *_Addend, long _Value) {
  return __atomic_add_fetch(_Addend, _Value, 0) - _Value;
}
#ifdef __x86_64__
static __inline__ __int64 __attribute__((__always_inline__, __nodebug__))
_InterlockedExchangeAdd64(__int64 volatile *_Addend, __int64 _Value) {
  return __atomic_add_fetch(_Addend, _Value, 0) - _Value;
}
#endif
/*----------------------------------------------------------------------------*\
|* Interlocked Exchange Sub
\*----------------------------------------------------------------------------*/
static __inline__ char __attribute__((__always_inline__, __nodebug__))
_InterlockedExchangeSub8(char volatile *_Subend, char _Value) {
  return __atomic_sub_fetch(_Subend, _Value, 0) + _Value;
}
static __inline__ short __attribute__((__always_inline__, __nodebug__))
_InterlockedExchangeSub16(short volatile *_Subend, short _Value) {
  return __atomic_sub_fetch(_Subend, _Value, 0) + _Value;
}
static __inline__ long __attribute__((__always_inline__, __nodebug__))
_InterlockedExchangeSub(long volatile *_Subend, long _Value) {
  return __atomic_sub_fetch(_Subend, _Value, 0) + _Value;
}
#ifdef __x86_64__
static __inline__ __int64 __attribute__((__always_inline__, __nodebug__))
_InterlockedExchangeSub64(__int64 volatile *_Subend, __int64 _Value) {
  return __atomic_sub_fetch(_Subend, _Value, 0) + _Value;
}
#endif
/*----------------------------------------------------------------------------*\
|* Interlocked Increment
\*----------------------------------------------------------------------------*/
static __inline__ char __attribute__((__always_inline__, __nodebug__))
_InterlockedIncrement16(char volatile *_Value) {
  return __atomic_add_fetch(_Value, 1, 0);
}
static __inline__ long __attribute__((__always_inline__, __nodebug__))
_InterlockedIncrement(long volatile *_Value) {
  return __atomic_add_fetch(_Value, 1, 0);
}
#ifdef __x86_64__
static __inline__ __int64 __attribute__((__always_inline__, __nodebug__))
_InterlockedIncrement64(__int64 volatile *_Value) {
  return __atomic_add_fetch(_Value, 1, 0);
}
#endif
/*----------------------------------------------------------------------------*\
|* Interlocked Decrement
\*----------------------------------------------------------------------------*/
static __inline__ char __attribute__((__always_inline__, __nodebug__))
_InterlockedDecrement16(char volatile *_Value) {
  return __atomic_sub_fetch(_Value, 1, 0);
}
static __inline__ long __attribute__((__always_inline__, __nodebug__))
_InterlockedDecrement(long volatile *_Value) {
  return __atomic_sub_fetch(_Value, 1, 0);
}
#ifdef __x86_64__
static __inline__ __int64 __attribute__((__always_inline__, __nodebug__))
_InterlockedDecrement64(__int64 volatile *_Value) {
  return __atomic_sub_fetch(_Value, 1, 0);
}
#endif
/*----------------------------------------------------------------------------*\
|* Interlocked And
\*----------------------------------------------------------------------------*/
static __inline__ char __attribute__((__always_inline__, __nodebug__))
_InterlockedAnd8(char volatile *_Value, char _Mask) {
  return __atomic_and_fetch(_Value, _Mask, 0);
}
static __inline__ short __attribute__((__always_inline__, __nodebug__))
_InterlockedAnd16(short volatile *_Value, short _Mask) {
  return __atomic_and_fetch(_Value, _Mask, 0);
}
static __inline__ long __attribute__((__always_inline__, __nodebug__))
_InterlockedAnd(long volatile *_Value, long _Mask) {
  return __atomic_and_fetch(_Value, _Mask, 0);
}
#ifdef __x86_64__
static __inline__ __int64 __attribute__((__always_inline__, __nodebug__))
_InterlockedAnd64(__int64 volatile *_Value, __int64 _Mask) {
  return __atomic_and_fetch(_Value, _Mask, 0);
}
#endif
/*----------------------------------------------------------------------------*\
|* Interlocked Or
\*----------------------------------------------------------------------------*/
static __inline__ char __attribute__((__always_inline__, __nodebug__))
_InterlockedOr8(char volatile *_Value, char _Mask) {
  return __atomic_or_fetch(_Value, _Mask, 0);
}
static __inline__ short __attribute__((__always_inline__, __nodebug__))
_InterlockedOr16(short volatile *_Value, short _Mask) {
  return __atomic_or_fetch(_Value, _Mask, 0);
}
static __inline__ long __attribute__((__always_inline__, __nodebug__))
_InterlockedOr(long volatile *_Value, long _Mask) {
  return __atomic_or_fetch(_Value, _Mask, 0);
}
#ifdef __x86_64__
static __inline__ __int64 __attribute__((__always_inline__, __nodebug__))
_InterlockedOr64(__int64 volatile *_Value, __int64 _Mask) {
  return __atomic_or_fetch(_Value, _Mask, 0);
}
#endif
/*----------------------------------------------------------------------------*\
|* Interlocked Xor
\*----------------------------------------------------------------------------*/
static __inline__ char __attribute__((__always_inline__, __nodebug__))
_InterlockedXor8(char volatile *_Value, char _Mask) {
  return __atomic_xor_fetch(_Value, _Mask, 0);
}
static __inline__ short __attribute__((__always_inline__, __nodebug__))
_InterlockedXor16(short volatile *_Value, short _Mask) {
  return __atomic_xor_fetch(_Value, _Mask, 0);
}
static __inline__ long __attribute__((__always_inline__, __nodebug__))
_InterlockedXor(long volatile *_Value, long _Mask) {
  return __atomic_xor_fetch(_Value, _Mask, 0);
}
#ifdef __x86_64__
static __inline__ __int64 __attribute__((__always_inline__, __nodebug__))
_InterlockedXor64(__int64 volatile *_Value, __int64 _Mask) {
  return __atomic_xor_fetch(_Value, _Mask, 0);
}
#endif
/*----------------------------------------------------------------------------*\
|* Interlocked Exchange
\*----------------------------------------------------------------------------*/
static __inline__ char __attribute__((__always_inline__, __nodebug__))
_InterlockedExchange8(char volatile *_Target, char _Value) {
  __atomic_exchange(_Target, &_Value, &_Value, 0);
  return _Value;
}
static __inline__ short __attribute__((__always_inline__, __nodebug__))
_InterlockedExchange16(short volatile *_Target, short _Value) {
  __atomic_exchange(_Target, &_Value, &_Value, 0);
  return _Value;
}
static __inline__ long __attribute__((__always_inline__, __nodebug__))
_InterlockedExchange(long volatile *_Target, long _Value) {
  __atomic_exchange(_Target, &_Value, &_Value, 0);
  return _Value;
}
#ifdef __x86_64__
static __inline__ __int64 __attribute__((__always_inline__, __nodebug__))
_InterlockedExchange64(__int64 volatile *_Target, __int64 _Value) {
  __atomic_exchange(_Target, &_Value, &_Value, 0);
  return _Value;
}
#endif
/*----------------------------------------------------------------------------*\
|* Interlocked Compare Exchange
\*----------------------------------------------------------------------------*/
static __inline__ char __attribute__((__always_inline__, __nodebug__))
_InterlockedCompareExchange8(char volatile *_Destination,
                             char _Exchange, char _Comparand) {
  __atomic_compare_exchange(_Destination, &_Comparand, &_Exchange, 0, 0, 0);
  return _Comparand;
}
static __inline__ short __attribute__((__always_inline__, __nodebug__))
_InterlockedCompareExchange16(short volatile *_Destination,
                              short _Exchange, short _Comparand) {
  __atomic_compare_exchange(_Destination, &_Comparand, &_Exchange, 0, 0, 0);
  return _Comparand;
}
static __inline__ long __attribute__((__always_inline__, __nodebug__))
_InterlockedCompareExchange(long volatile *_Destination,
                            long _Exchange, long _Comparand) {
  __atomic_compare_exchange(_Destination, &_Comparand, &_Exchange, 0, 0, 0);
  return _Comparand;
}
#ifdef __x86_64__
static __inline__ __int64 __attribute__((__always_inline__, __nodebug__))
_InterlockedCompareExchange64(__int64 volatile *_Destination,
                              __int64 _Exchange, __int64 _Comparand) {
  __atomic_compare_exchange(_Destination, &_Comparand, &_Exchange, 0, 0, 0);
  return _Comparand;
}
#endif
/*----------------------------------------------------------------------------*\
|* Barriers
\*----------------------------------------------------------------------------*/
static __inline__ void __attribute__((__always_inline__, __nodebug__))
__attribute__((deprecated("use other intrinsics or C++11 atomics instead")))
_ReadWriteBarrier(void) {
  __asm__ volatile ("" : : : "memory");
}
static __inline__ void __attribute__((__always_inline__, __nodebug__))
__attribute__((deprecated("use other intrinsics or C++11 atomics instead")))
_ReadBarrier(void) {
  __asm__ volatile ("" : : : "memory");
}
static __inline__ void __attribute__((__always_inline__, __nodebug__))
__attribute__((deprecated("use other intrinsics or C++11 atomics instead")))
_WriteBarrier(void) {
  __asm__ volatile ("" : : : "memory");
}
/*----------------------------------------------------------------------------*\
|* Misc
\*----------------------------------------------------------------------------*/
static __inline__ void * __attribute__((__always_inline__, __nodebug__))
_AddressOfReturnAddress(void) {
  return (void*)((char*)__builtin_frame_address(0) + sizeof(void*));
}
static __inline__ void * __attribute__((__always_inline__, __nodebug__))
_ReturnAddress(void) {
  return __builtin_return_address(0);
}

#ifdef __cplusplus
}
#endif

#endif /* __INTRIN_H */
#endif /* _MSC_VER */
