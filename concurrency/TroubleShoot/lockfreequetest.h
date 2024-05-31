#include <iostream>
#include <thread>
#include "lockfreeque.h"
#include <cassert>
#include "crushque.h"
#include "memoryleakque.h"

#define TESTCOUNT 10

extern void TestLockFreeQueBase();

extern void TestLockFreeQue();

extern void TestLockFreeQueMultiPop();

extern void TestLockFreeQueMultiPushPop();

extern void TestCrushQue();

extern void TestLeakQue();

extern void TestLeakQueSingleThread();

extern void TestLeakQueMultiPop();

extern void TestLockFreeQueMultiPushPop2();



