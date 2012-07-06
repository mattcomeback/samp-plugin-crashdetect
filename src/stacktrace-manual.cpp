// Copyright (c) 2011-2012, Zeex
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>

#include "compiler.h"
#include "stacktrace-manual.h"

static inline void *GetReturnAddress(void *frmAddr) {
	return *reinterpret_cast<void**>(reinterpret_cast<char*>(frmAddr) + 4);
}

static inline void *GetNextFrame(void *frmAddr) {
	return *reinterpret_cast<void**>(frmAddr);
}

StackTraceManual::StackTraceManual(void *frame, void *pc)
	: StackTrace(static_cast<HappyCompiler*>(0))
{
	void *curFrame = frame == 0
		? compiler::GetFrameAddress()
		: frame;

	void *top = compiler::GetStackTop();
	void *bot = compiler::GetStackBottom();

	for (int i = 0; ; i++) {
		if (curFrame == 0
			|| (curFrame >= top && top != 0)
			|| (curFrame < bot && bot != 0)) {
			break;
		}

		void *ret = GetReturnAddress(curFrame);
		if (ret == 0) {
			break;
		}

		curFrame = GetNextFrame(curFrame);
		frames_.push_back(StackFrame(ret));
	}
}
