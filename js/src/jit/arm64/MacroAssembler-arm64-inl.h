/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 * vim: set ts=8 sts=4 et sw=4 tw=99:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef jit_arm64_MacroAssembler_arm64_inl_h
#define jit_arm64_MacroAssembler_arm64_inl_h

#include "jit/arm64/MacroAssembler-arm64.h"

namespace js {
namespace jit {

//{{{ check_macroassembler_style
// ===============================================================
// Logical instructions

void
MacroAssembler::not32(Register reg)
{
    Orn(ARMRegister(reg, 32), vixl::wzr, ARMRegister(reg, 32));
}

void
MacroAssembler::and32(Register src, Register dest)
{
    And(ARMRegister(dest, 32), ARMRegister(dest, 32), Operand(ARMRegister(src, 32)));
}

void
MacroAssembler::and32(Imm32 imm, Register dest)
{
    And(ARMRegister(dest, 32), ARMRegister(dest, 32), Operand(imm.value));
}

void
MacroAssembler::and32(Imm32 imm, Register src, Register dest)
{
    And(ARMRegister(dest, 32), ARMRegister(src, 32), Operand(imm.value));
}

void
MacroAssembler::and32(Imm32 imm, const Address& dest)
{
    vixl::UseScratchRegisterScope temps(this);
    const ARMRegister scratch32 = temps.AcquireW();
    MOZ_ASSERT(scratch32.asUnsized() != dest.base);
    load32(dest, scratch32.asUnsized());
    And(scratch32, scratch32, Operand(imm.value));
    store32(scratch32.asUnsized(), dest);
}

void
MacroAssembler::and32(const Address& src, Register dest)
{
    vixl::UseScratchRegisterScope temps(this);
    const ARMRegister scratch32 = temps.AcquireW();
    MOZ_ASSERT(scratch32.asUnsized() != src.base);
    load32(src, scratch32.asUnsized());
    And(ARMRegister(dest, 32), ARMRegister(dest, 32), Operand(scratch32));
}

void
MacroAssembler::andPtr(Register src, Register dest)
{
    And(ARMRegister(dest, 64), ARMRegister(dest, 64), Operand(ARMRegister(src, 64)));
}

void
MacroAssembler::andPtr(Imm32 imm, Register dest)
{
    And(ARMRegister(dest, 64), ARMRegister(dest, 64), Operand(imm.value));
}

void
MacroAssembler::and64(Imm64 imm, Register64 dest)
{
    vixl::UseScratchRegisterScope temps(this);
    const Register scratch = temps.AcquireX().asUnsized();
    mov(ImmWord(imm.value), scratch);
    andPtr(scratch, dest.reg);
}

void
MacroAssembler::or32(Imm32 imm, Register dest)
{
    Orr(ARMRegister(dest, 32), ARMRegister(dest, 32), Operand(imm.value));
}

void
MacroAssembler::or32(Register src, Register dest)
{
    Orr(ARMRegister(dest, 32), ARMRegister(dest, 32), Operand(ARMRegister(src, 32)));
}

void
MacroAssembler::or32(Imm32 imm, const Address& dest)
{
    vixl::UseScratchRegisterScope temps(this);
    const ARMRegister scratch32 = temps.AcquireW();
    MOZ_ASSERT(scratch32.asUnsized() != dest.base);
    load32(dest, scratch32.asUnsized());
    Orr(scratch32, scratch32, Operand(imm.value));
    store32(scratch32.asUnsized(), dest);
}

void
MacroAssembler::orPtr(Register src, Register dest)
{
    Orr(ARMRegister(dest, 64), ARMRegister(dest, 64), Operand(ARMRegister(src, 64)));
}

void
MacroAssembler::orPtr(Imm32 imm, Register dest)
{
    Orr(ARMRegister(dest, 64), ARMRegister(dest, 64), Operand(imm.value));
}

void
MacroAssembler::or64(Register64 src, Register64 dest)
{
    orPtr(src.reg, dest.reg);
}

void
MacroAssembler::xor64(Register64 src, Register64 dest)
{
    xorPtr(src.reg, dest.reg);
}

void
MacroAssembler::xor32(Imm32 imm, Register dest)
{
    Eor(ARMRegister(dest, 32), ARMRegister(dest, 32), Operand(imm.value));
}

void
MacroAssembler::xorPtr(Register src, Register dest)
{
    Eor(ARMRegister(dest, 64), ARMRegister(dest, 64), Operand(ARMRegister(src, 64)));
}

void
MacroAssembler::xorPtr(Imm32 imm, Register dest)
{
    Eor(ARMRegister(dest, 64), ARMRegister(dest, 64), Operand(imm.value));
}

// ===============================================================
// Arithmetic functions

void
MacroAssembler::add32(Register src, Register dest)
{
    Add(ARMRegister(dest, 32), ARMRegister(dest, 32), Operand(ARMRegister(src, 32)));
}

void
MacroAssembler::add32(Imm32 imm, Register dest)
{
    Add(ARMRegister(dest, 32), ARMRegister(dest, 32), Operand(imm.value));
}

void
MacroAssembler::add32(Imm32 imm, const Address& dest)
{
    vixl::UseScratchRegisterScope temps(this);
    const ARMRegister scratch32 = temps.AcquireW();
    MOZ_ASSERT(scratch32.asUnsized() != dest.base);

    Ldr(scratch32, MemOperand(ARMRegister(dest.base, 64), dest.offset));
    Add(scratch32, scratch32, Operand(imm.value));
    Str(scratch32, MemOperand(ARMRegister(dest.base, 64), dest.offset));
}

void
MacroAssembler::addPtr(Register src, Register dest)
{
    addPtr(src, dest, dest);
}

void
MacroAssembler::addPtr(Register src1, Register src2, Register dest)
{
    Add(ARMRegister(dest, 64), ARMRegister(src1, 64), Operand(ARMRegister(src2, 64)));
}

void
MacroAssembler::addPtr(Imm32 imm, Register dest)
{
    addPtr(imm, dest, dest);
}

void
MacroAssembler::addPtr(Imm32 imm, Register src, Register dest)
{
    Add(ARMRegister(dest, 64), ARMRegister(src, 64), Operand(imm.value));
}

void
MacroAssembler::addPtr(ImmWord imm, Register dest)
{
    Add(ARMRegister(dest, 64), ARMRegister(dest, 64), Operand(imm.value));
}

void
MacroAssembler::addPtr(Imm32 imm, const Address& dest)
{
    vixl::UseScratchRegisterScope temps(this);
    const ARMRegister scratch64 = temps.AcquireX();
    MOZ_ASSERT(scratch64.asUnsized() != dest.base);

    Ldr(scratch64, MemOperand(ARMRegister(dest.base, 64), dest.offset));
    Add(scratch64, scratch64, Operand(imm.value));
    Str(scratch64, MemOperand(ARMRegister(dest.base, 64), dest.offset));
}

void
MacroAssembler::addPtr(const Address& src, Register dest)
{
    vixl::UseScratchRegisterScope temps(this);
    const ARMRegister scratch64 = temps.AcquireX();
    MOZ_ASSERT(scratch64.asUnsized() != src.base);

    Ldr(scratch64, MemOperand(ARMRegister(src.base, 64), src.offset));
    Add(ARMRegister(dest, 64), ARMRegister(dest, 64), Operand(scratch64));
}

void
MacroAssembler::add64(Register64 src, Register64 dest)
{
    addPtr(src.reg, dest.reg);
}

void
MacroAssembler::add64(Imm32 imm, Register64 dest)
{
    Add(ARMRegister(dest.reg, 64), ARMRegister(dest.reg, 64), Operand(imm.value));
}

void
MacroAssembler::addDouble(FloatRegister src, FloatRegister dest)
{
    fadd(ARMFPRegister(dest, 64), ARMFPRegister(dest, 64), ARMFPRegister(src, 64));
}

void
MacroAssembler::sub32(Imm32 imm, Register dest)
{
    Sub(ARMRegister(dest, 32), ARMRegister(dest, 32), Operand(imm.value));
}

void
MacroAssembler::sub32(Register src, Register dest)
{
    Sub(ARMRegister(dest, 32), ARMRegister(dest, 32), Operand(ARMRegister(src, 32)));
}

void
MacroAssembler::sub32(const Address& src, Register dest)
{
    vixl::UseScratchRegisterScope temps(this);
    const ARMRegister scratch32 = temps.AcquireW();
    MOZ_ASSERT(scratch32.asUnsized() != src.base);
    load32(src, scratch32.asUnsized());
    Sub(ARMRegister(dest, 32), ARMRegister(dest, 32), Operand(scratch32));
}

void
MacroAssembler::subPtr(Register src, Register dest)
{
    Sub(ARMRegister(dest, 64), ARMRegister(dest, 64), Operand(ARMRegister(src, 64)));
}

void
MacroAssembler::subPtr(Register src, const Address& dest)
{
    vixl::UseScratchRegisterScope temps(this);
    const ARMRegister scratch64 = temps.AcquireX();
    MOZ_ASSERT(scratch64.asUnsized() != dest.base);

    Ldr(scratch64, MemOperand(ARMRegister(dest.base, 64), dest.offset));
    Sub(scratch64, scratch64, Operand(ARMRegister(src, 64)));
    Str(scratch64, MemOperand(ARMRegister(dest.base, 64), dest.offset));
}

void
MacroAssembler::subPtr(Imm32 imm, Register dest)
{
    Sub(ARMRegister(dest, 64), ARMRegister(dest, 64), Operand(imm.value));
}

void
MacroAssembler::subPtr(const Address& addr, Register dest)
{
    vixl::UseScratchRegisterScope temps(this);
    const ARMRegister scratch64 = temps.AcquireX();
    MOZ_ASSERT(scratch64.asUnsized() != addr.base);

    Ldr(scratch64, MemOperand(ARMRegister(addr.base, 64), addr.offset));
    Sub(ARMRegister(dest, 64), ARMRegister(dest, 64), Operand(scratch64));
}

void
MacroAssembler::subDouble(FloatRegister src, FloatRegister dest)
{
    fsub(ARMFPRegister(dest, 64), ARMFPRegister(dest, 64), ARMFPRegister(src, 64));
}

void
MacroAssembler::mul32(Register src1, Register src2, Register dest, Label* onOver, Label* onZero)
{
    Smull(ARMRegister(dest, 64), ARMRegister(src1, 32), ARMRegister(src2, 32));
    if (onOver) {
        Cmp(ARMRegister(dest, 64), Operand(ARMRegister(dest, 32), vixl::SXTW));
        B(onOver, NotEqual);
    }
    if (onZero)
        Cbz(ARMRegister(dest, 32), onZero);

    // Clear upper 32 bits.
    Mov(ARMRegister(dest, 32), ARMRegister(dest, 32));
}

void
MacroAssembler::mul64(Imm64 imm, const Register64& dest)
{
    vixl::UseScratchRegisterScope temps(this);
    const ARMRegister scratch64 = temps.AcquireX();
    MOZ_ASSERT(dest.reg != scratch64.asUnsized());
    mov(ImmWord(imm.value), scratch64.asUnsized());
    Mul(ARMRegister(dest.reg, 64), ARMRegister(dest.reg, 64), scratch64);
}

void
MacroAssembler::mulBy3(Register src, Register dest)
{
    ARMRegister xdest(dest, 64);
    ARMRegister xsrc(src, 64);
    Add(xdest, xsrc, Operand(xsrc, vixl::LSL, 1));
}

void
MacroAssembler::mulDouble(FloatRegister src, FloatRegister dest)
{
    fmul(ARMFPRegister(dest, 64), ARMFPRegister(dest, 64), ARMFPRegister(src, 64));
}

void
MacroAssembler::mulDoublePtr(ImmPtr imm, Register temp, FloatRegister dest)
{
    vixl::UseScratchRegisterScope temps(this);
    const Register scratch = temps.AcquireX().asUnsized();
    MOZ_ASSERT(temp != scratch);
    movePtr(imm, scratch);
    const ARMFPRegister scratchDouble = temps.AcquireD();
    Ldr(scratchDouble, MemOperand(Address(scratch, 0)));
    fmul(ARMFPRegister(dest, 64), ARMFPRegister(dest, 64), scratchDouble);
}

void
MacroAssembler::divDouble(FloatRegister src, FloatRegister dest)
{
    fdiv(ARMFPRegister(dest, 64), ARMFPRegister(dest, 64), ARMFPRegister(src, 64));
}

void
MacroAssembler::inc64(AbsoluteAddress dest)
{
    vixl::UseScratchRegisterScope temps(this);
    const ARMRegister scratchAddr64 = temps.AcquireX();
    const ARMRegister scratch64 = temps.AcquireX();

    Mov(scratchAddr64, uint64_t(dest.addr));
    Ldr(scratch64, MemOperand(scratchAddr64, 0));
    Add(scratch64, scratch64, Operand(1));
    Str(scratch64, MemOperand(scratchAddr64, 0));
}

void
MacroAssembler::neg32(Register reg)
{
    Negs(ARMRegister(reg, 32), Operand(ARMRegister(reg, 32)));
}

void
MacroAssembler::negateFloat(FloatRegister reg)
{
    fneg(ARMFPRegister(reg, 32), ARMFPRegister(reg, 32));
}

void
MacroAssembler::negateDouble(FloatRegister reg)
{
    fneg(ARMFPRegister(reg, 64), ARMFPRegister(reg, 64));
}

// ===============================================================
// Shift functions

void
MacroAssembler::lshiftPtr(Imm32 imm, Register dest)
{
    Lsl(ARMRegister(dest, 64), ARMRegister(dest, 64), imm.value);
}

void
MacroAssembler::lshift64(Imm32 imm, Register64 dest)
{
    lshiftPtr(imm, dest.reg);
}

void
MacroAssembler::rshiftPtr(Imm32 imm, Register dest)
{
    Lsr(ARMRegister(dest, 64), ARMRegister(dest, 64), imm.value);
}

void
MacroAssembler::rshiftPtr(Imm32 imm, Register src, Register dest)
{
    Lsr(ARMRegister(dest, 64), ARMRegister(src, 64), imm.value);
}

void
MacroAssembler::rshiftPtrArithmetic(Imm32 imm, Register dest)
{
    Asr(ARMRegister(dest, 64), ARMRegister(dest, 64), imm.value);
}

void
MacroAssembler::rshift64(Imm32 imm, Register64 dest)
{
    rshiftPtr(imm, dest.reg);
}

//}}} check_macroassembler_style
// ===============================================================

template <typename T>
void
MacroAssemblerCompat::addToStackPtr(T t)
{
    asMasm().addPtr(t, getStackPointer());
}

template <typename T>
void
MacroAssemblerCompat::addStackPtrTo(T t)
{
    asMasm().addPtr(getStackPointer(), t);
}

template <typename T>
void
MacroAssemblerCompat::subFromStackPtr(T t)
{
    asMasm().subPtr(t, getStackPointer()); syncStackPtr();
}

template <typename T>
void
MacroAssemblerCompat::subStackPtrFrom(T t)
{
    asMasm().subPtr(getStackPointer(), t);
}

template <typename T>
void
MacroAssemblerCompat::andToStackPtr(T t)
{
    asMasm().andPtr(t, getStackPointer());
    syncStackPtr();
}

template <typename T>
void
MacroAssemblerCompat::andStackPtrTo(T t)
{
    asMasm().andPtr(getStackPointer(), t);
}

} // namespace jit
} // namespace js

#endif /* jit_arm64_MacroAssembler_arm64_inl_h */
