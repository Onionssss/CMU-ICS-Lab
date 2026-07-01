/* 
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here>
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting if the shift amount
     is less than 0 or greater than 31.


EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implement floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants. You can use any arithmetic,
logical, or comparison operations on int or unsigned data.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operations (integer, logical,
     or comparison) that you are allowed to use for your implementation
     of the function.  The max operator count is checked by dlc.
     Note that assignment ('=') is not counted; you may use as many of
     these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
//1
/* 
 * bitXor - x^y using only ~ and & 
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y) { // because this is not ANSI C,
  // so just declare the variable type and assign the value directly in the same line is totally fine
  // let's do abstraction
  int a = x & ~y;
  int b = ~x & y;
  // x xor y == (x & ~y) | (~x & y)
  // a or b == ~(~a & ~b)
  return ~(~a & ~b);
}
/* 
 * tmin - return minimum two's complement integer
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
  // the minimum integer in two's complement
  // 1000 0000 ... 0000
  // 0x 8000 0000
  // but we cannot just return the number, it's resricted
  return 1 << 31;
}
//2
/*
 * isTmax - returns 1 if x is the maximum, two's complement number,
 *     and 0 otherwise 
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int isTmax(int x) {
  // let x = 0x7FFF FFFF, the max value
  // x + 1 = 0x1000 0000, which is complement
  // ~(x + 1) = 0x7FFF FFFF = x
  // x ^ ~(x + 1) = 0, if they are the same number
  // !(x ^ ~(x+1)) = 1, this will return 1
  int plus_one = x + 1;
  int is_complement = !(x ^ ~(plus_one));
  // to test if x is -1
  // if (tmp == 0) return 1
  // === !tmp
  int is_not_minus_one = !!plus_one; // will return 0 if plus_one is 0, which makes x originally -1
  return is_complement & is_not_minus_one;
}
/* 
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allOddBits(int x) {
  // information compress into 8 bit
  x = x & (x >> 16); // first half of 16 bits AND second half of 16 bits
  x = x & (x >> 8); // the way with 8 bits
  return !((x & 0xAA) ^ 0xAA);
}
/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
  return ~x + 1; // 2's complement form of * (-1)
}
//3
/* 
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x) {
  // divide into 2 conditions:
  // 1. if x >= 0x30 -> x - 0x30 >= 0
  // 2. if x <= 0x39 -> 0x39 - x >= 0
  int cond1 = x + (~0x30 + 1); // x - 0x30
  int cond2 = 0x39 + (~x + 1); // 0x39 - x
  cond1 = (cond1 >> 31) & 1; // only look for rightmsot bit
  cond2 = (cond2 >> 31) & 1;
  return !(cond1 | cond2); // both of them have to be positive or zero to get 0
}
/* 
 * conditional - same as x ? y : z 
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {
  // if (x) return y; else return z;
  /*according to the true and false value of x, 
  generate the mask: 
  true -> 0xFFFF FFFF, which is -1
  false -> 0x0000 0000, 0
  */
  int mask = !!x; // true: return 1, false: return 0
  mask = ~mask + 1; // 2's complement form of * (-1)
  return (mask & y) | (~mask & z);
}
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
  // to avoid integer overflow, make a sign comparison first
  #define GET_SIGN(val) (((val) >> 31) & 1)
  int sign_x = GET_SIGN(x); // 1 means negative
  int sign_y = GET_SIGN(y); // 0 means positive or 0

  int is_opp_sign_safe = sign_x & !(sign_y); // x < 0 and y > 0 -> x <= y, return 1
  int same_sign = !(sign_x ^ sign_y); // 1 means same, 0 means opposite
  // x - y <= 0 -> y - x >= 0
  int res = y + (~x + 1); // y - x
  int res_sign = GET_SIGN(res);

  int is_same_sign_safe = same_sign & !(res_sign);
  return is_opp_sign_safe | is_same_sign_safe;
  #undef GET_SIGN
}
//4
/* 
 * logicalNeg - implement the ! operator, using all of 
 *              the legal operators except !
 *   Examples: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int logicalNeg(int x) {
  x = x | (~x + 1); // let x or -x, only 0 itself will get 0
  return (x >> 31) + 1;
}
/* howManyBits - return the minimum number of bits required to represent x in
 *             two's complement
 *  Examples: howManyBits(12) = 5
 *            howManyBits(298) = 10
 *            howManyBits(-5) = 4
 *            howManyBits(0)  = 1
 *            howManyBits(-1) = 1
 *            howManyBits(0x80000000) = 32
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 90
 *  Rating: 4
 */
int howManyBits(int x) {
  int sign_x = x >> 31; // x < 0: 0xFFFF FFFF, x >= 0:0x0000 0000
  x = x ^ sign_x; // this will invert the negative number, or will remain positive number as it was
  // then this problem will turn into:
  // what's the exact position of the leftmost 1, and plus one(to represent sign itself) is the answer
  // use binary search to make the whole number half each time
  int b16 = (!!(x >> 16)) << 4; // if highest 16 bits has 1, make b16 = 16 in binary(b16 is a number here)
  // otherwise, make it 0
  x = x >> b16; // remember to assign bit shift to x

  int b8 = (!!(x >> 8)) << 3;
  x = x >> b8;

  int b4 = (!!(x >> 4)) << 2;
  x = x >> b4;

  int b2 = (!!(x >> 2)) << 1;
  x = x >> b2;

  int b1 = (!!(x >> 1)) << 0;
  x = x >> b1;

  int b0 = x;
  return b16 + b8 + b4 + b2 + b1 + b0 + 1; // 1 is for sign bit
}
//float
/* 
 * floatScale2 - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned floatScale2(unsigned uf) {
  // already in the IEEE 754 form
  // sign: 1, exp: 8, frac: 23 bits
  int sign = (uf >> 31) & 1;
  int exp = (uf >> 23) & 0xFF; // only take 8 bits
  int frac = uf & 0x7FFFFF;

  if (exp == 0) { // denormalize number, frac part << 1
    frac = frac << 1;
  }
  else if (exp == 0xFF) { // Inf or NaN
    return uf;
  }
  else { // normalize number
    exp++;
  }

  return (sign << 31) | (exp << 23) | frac;
}
/* 
 * floatFloat2Int - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
int floatFloat2Int(unsigned uf) {
  int sign = (uf >> 31) & 1; // sign: 1
  int exp = (uf >> 23) & 0xFF; // exp: 8
  int frac = uf & 0x7FFFFF; // frac: 23 bits
  int mantissa = frac | (1 << 23); // 1.F * 2^23 for now
  int E = exp - 127;
  
  if (E < 0) { // denormalize number
    return 0;
  }
  // normalize number
  // E = 3, 1.010 * 2^3 = 1010, which is 10 in binary
  // since we've already shift our mantissa at least 23 bits
  // we need to shift the exponent back to 3
  // which means >> (23 - E), which is 20 in this case
  else if (0 <= E && E < 23) {
    mantissa = mantissa >> (23 - E);
  }
  else if (23 <= E && E < 31) { 
    mantissa = mantissa << (E - 23);
  }
  else { // E >= 31, too large
    return 0x80000000u;
  }

  if (sign) {
    return ~mantissa + 1;
  }
  return mantissa;
}
/* 
 * floatPower2 - Return bit-level equivalent of the expression 2.0^x
 *   (2.0 raised to the power x) for any 32-bit integer x.
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^x.
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 * 
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. Also if, while 
 *   Max ops: 30
 *   Rating: 4
 */
unsigned floatPower2(int x) {
  if (x <= -149){ // less than denormalize number
    return 0;
  }
  else if (x <= -127) { // denormalize number
    return 1 << (x + 149);
  }
  else if (x <= 127) { // normalize number
    int exp = x + 127;
    return exp << 23;
  }
  else { // too large, return +INF, 0x7F80 0000
    return 0xFF << 23;
  }
}
