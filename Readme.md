# Summary

- [Introduction](#Introduction)
- [Build](#Build)
- [Style](#Style)
- [Examples](#Examples)
- [Architecture](#architecture)
- [Memory Usage explanation](#Memory-Usage-explanation)

# Introduction

SimpleCompactString it's a very naive and simple implementation made by me, only for learning purposes.

# Build

To compile, you will need at least a compatible **c11** compiler.  
Also, you will need `cmake` to build the project.

Example:    
```txt
cmake -B build/ -DCMAKE_BUILD_TYPE=Debug  
cmake --build build/
```

# Style

I like to define my own formatter. So I use the LLVM coding standard, however if applied some modifications.    
You can check the format, by issuing the command: `clang-format -style=file`

Example: `clang-format -style=file --ferror-limit=0 --dry-run include/scs/*.h`, this will run the check in the headers file, and will return error when an 
violation its found.

To format the files: `clang-format -style=file -i include/scs/*.h source/scs/*.c`, this will format(edit in places the files) using
the custom format define in this directory.

# Examples

There are some working examples that show the simplest usage of **scs**.

# Architecture

This section explains the architecture of SCS, what are the motivations, some examples and so on.   
Its kinda of different of the C strings, and usually common implementations. 
If we compare, usually we see some libraries that uses 4 bytes to store the string length, and then, store the string data.  
This usually solves their problem, however, in some applications we are not going to have strings with more 
than 255(1 byte) or 65535(2 bytes) of length.  
So we are wasting 2 or even 3 bytes.  
Other implementations might use Base128 for example, however it can be some wasteful for some scenarios and also, you need to process the length all the time,
even with cache lines, this will cause some cycles of the processor.  

**SCS** does something different, we combine **variable length number**(to represent the `input buffer) plus one byte to be the **options**(quantity of bytes needed to store the input size, if the string its immutable, encode type).

Lets get some examples to show: 

- The string `Hello`

  The string has five characters plus one to represent the null byte(because we also want a c stdlib compatible).   
  The number six, fits in one byte.  

  Lets say that we requested memory 8 bytes from the OS, and it returned the **region** `0x4052a0` to us.

  | Memory Region |   Value   |
  |:-------------:|:---------:|
  |   0x4052A0    | 0x06(The length of the input string plus the null character fits in one byte)|   
  |   0x4052A1    | 0x09(The options) |   
  |   0x4052A2    | 0x48('H') |
  |   0x4052A3    | 0x65('e') |
  |   0x4052A4    | 0x6C('l') |
  |   0x4052A5    | 0x6C('l') |
  |   0x4052A6    | 0x6F('o') |  
  |   0x4052A7    | 0x00(The null byte) |  


  After this allocation, we apply some pointer arithmetics and return the address: `0x4052A2` to the caller, so, it still has a c compatible string.


Also, **SCS** its cache friendly, we do only one allocation.
Usually, common libraries do: 
```c
struct string
{
    char *buffer;
};
```
Hence, we will need 2 allocations, and since both are on the heap, probably, when (string->buffer), likely, we are going to have a cache-miss.   
**SCS** adds a new data type called `scs`, which is just an typedef to `void *`.

To create text string, you can use: `scs_from_string`, and to create strings from non text arrays, you can use: `scs_from`


# Memory Usage explanation

The first bytes(min(1), max(8)) are used to store the variable length string size.   
The following byte its used to store the string options.

- ## Strings with $size \lt 256$

  For this type of string, only two additional bytes are going to be used.   
  The first byte, will be the options, the second byte will be the string length.

- ## Strings with $256 \ge size \lt 65536$

    For this type of string, only three additional bytes are going to be used.   
    The first byte, will be the options, the second and the third byte represent the string length.

- ## Strings with $65536 \ge size \lt 16777216$

    For this type of string, only four additional bytes are going to be used.   
    The first byte, will be the options, the second, third  and the fourth byte represent the string length.

- ## Strings with $16777216 \ge size \lt 4294967296$

    For this type of string, only five additional bytes are going to be used.   
    The first byte, will be the options, the second, third, fourth and the fifth byte represent the string length.
