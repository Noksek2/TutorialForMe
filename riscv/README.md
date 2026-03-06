# RISC-V 정리 (시스템 프로그래밍 수준)

---

## 1. RISC-V 기본 철학 & 설계 원칙

### RISC-V란?
- **Berkeley에서 2010년대 시작**한 오픈소스 ISA (Instruction Set Architecture)
- **Modular ISA**: 기본(Base) + 선택적 확장(Extension)으로 구성
- **Little-endian** 기본 (Big-endian도 선택 가능)
- **고정 32비트 명령어** 폭 (Compressed 확장 시 16비트도 가능)
- **레지스터-레지스터** 연산 중심 (메모리 직접 연산 없음)
- **조건부 코드(flags) 없음** — NZCV 같은 PSR 없음
- **delayed branch 없음** — MIPS와 다름

### ISA 종류
| 이름 | 설명 |
|------|------|
| RV32I | 32비트 정수 기본 ISA |
| RV64I | 64비트 정수 기본 ISA |
| RV128I | 128비트 (아직 실험적) |
| RV32E | 임베디드용, 레지스터 16개만 |

---

## 2. 레지스터 구조 (RV32I / RV64I)

### 정수 레지스터 (32개)

```
x0  = zero   항상 0, 쓰기 무시
x1  = ra     Return Address (링크 레지스터)
x2  = sp     Stack Pointer
x3  = gp     Global Pointer (linker가 세팅)
x4  = tp     Thread Pointer (TLS base)
x5  = t0     임시 레지스터 / 대체 링크 레지스터
x6  = t1     임시
x7  = t2     임시
x8  = s0/fp  Saved / Frame Pointer
x9  = s1     Saved
x10 = a0     함수 인자 0 / 반환값 0
x11 = a1     함수 인자 1 / 반환값 1
x12 = a2     함수 인자 2
x13 = a3     함수 인자 3
x14 = a4     함수 인자 4
x15 = a5     함수 인자 5
x16 = a6     함수 인자 6
x17 = a7     함수 인자 7
x18 = s2     Saved (callee-saved)
x19 = s3     Saved
x20 = s4     Saved
x21 = s5     Saved
x22 = s6     Saved
x23 = s7     Saved
x24 = s8     Saved
x25 = s9     Saved
x26 = s10    Saved
x27 = s11    Saved
x28 = t3     임시 (caller-saved)
x29 = t4     임시
x30 = t5     임시
x31 = t6     임시
```

> **x0는 쓰레기통**: `mv rd, rs` → `addi rd, rs, 0`  
> **x0에 쓰기**: 완전히 무시됨 (하드웨어 레벨에서)

### PC (Program Counter)
- **별도 레지스터** — 정수 레지스터 파일과 분리
- 직접 읽기 불가 (auipc으로 상대 주소 계산)

### 콜링 컨벤션 요약

| 종류 | 레지스터 | 책임 |
|------|----------|------|
| Caller-saved | t0-t6, a0-a7, ra | 호출자가 필요 시 저장 |
| Callee-saved | s0-s11, sp | 피호출자가 복원 보장 |
| 반환값 | a0, a1 | 최대 2개 워드 |
| 인자 | a0-a7 | 8개 초과 시 스택 |

---

## 3. 명령어 포맷 (Encoding)

### 6가지 기본 포맷

```
31      25 24  20 19  15 14  12 11   7 6      0
┌─────────┬──────┬──────┬──────┬──────┬────────┐
│  funct7 │  rs2 │  rs1 │funct3│  rd  │ opcode │  R-type
└─────────┴──────┴──────┴──────┴──────┴────────┘

┌─────────────────┬──────┬──────┬──────┬────────┐
│    imm[11:0]    │  rs1 │funct3│  rd  │ opcode │  I-type
└─────────────────┴──────┴──────┴──────┴────────┘

┌──────────┬──────┬──────┬──────┬──────┬────────┐
│ imm[11:5]│  rs2 │  rs1 │funct3│imm4:0│ opcode │  S-type
└──────────┴──────┴──────┴──────┴──────┴────────┘

┌─┬───────┬──────┬──────┬──────┬──────┬────────┐
│i│imm10:5│  rs2 │  rs1 │funct3│imm4:1│ opcode │  B-type (branch)
└─┴───────┴──────┴──────┴──────┴──────┴────────┘

┌──────────────────────────────┬──────┬────────┐
│          imm[31:12]          │  rd  │ opcode │  U-type
└──────────────────────────────┴──────┴────────┘

┌─┬──────────┬─┬──────────┬──────┬────────────┐
│i│ imm10:1  │i│ imm19:12 │  rd  │   opcode   │  J-type (jal)
└─┴──────────┴─┴──────────┴──────┴────────────┘
```

### 즉치값(Immediate) 부호 확장
- **항상 MSB에서 부호 확장** (sign-extended)
- B-type: 13비트 분기 오프셋 (bit[0]=0, 2바이트 정렬)
- J-type: 21비트 점프 오프셋 (bit[0]=0)

---

## 4. RV32I 기본 명령어 전체

### 산술/논리 연산 (R-type)

```asm
add   rd, rs1, rs2      # rd = rs1 + rs2
sub   rd, rs1, rs2      # rd = rs1 - rs2
and   rd, rs1, rs2      # rd = rs1 & rs2
or    rd, rs1, rs2      # rd = rs1 | rs2
xor   rd, rs1, rs2      # rd = rs1 ^ rs2
sll   rd, rs1, rs2      # rd = rs1 << rs2[4:0]  (논리 좌시프트)
srl   rd, rs1, rs2      # rd = rs1 >> rs2[4:0]  (논리 우시프트)
sra   rd, rs1, rs2      # rd = rs1 >> rs2[4:0]  (산술 우시프트, 부호 유지)
slt   rd, rs1, rs2      # rd = (rs1 < rs2) ? 1 : 0  (signed)
sltu  rd, rs1, rs2      # rd = (rs1 < rs2) ? 1 : 0  (unsigned)
```

### 즉치 연산 (I-type)

```asm
addi  rd, rs1, imm      # rd = rs1 + sign_ext(imm12)
andi  rd, rs1, imm      # rd = rs1 & sign_ext(imm12)
ori   rd, rs1, imm      # rd = rs1 | sign_ext(imm12)
xori  rd, rs1, imm      # rd = rs1 ^ sign_ext(imm12)
slli  rd, rs1, shamt    # rd = rs1 << shamt  (shamt: 5비트)
srli  rd, rs1, shamt    # rd = rs1 >> shamt  (논리)
srai  rd, rs1, shamt    # rd = rs1 >> shamt  (산술)
slti  rd, rs1, imm      # signed compare
sltiu rd, rs1, imm      # unsigned compare (imm은 부호확장 후 unsigned 비교)
```

> **sltiu rd, rs, 1** → rs==0이면 rd=1 (사실상 `seqz rd, rs`)

### 상위 즉치 (U-type)

```asm
lui   rd, imm20         # rd = imm20 << 12  (하위 12비트 = 0)
auipc rd, imm20         # rd = PC + (imm20 << 12)
```

**32비트 상수 로드 패턴:**
```asm
# 0xDEADBEEF 로드
lui   a0, 0xDEADB       # a0 = 0xDEADB000
addi  a0, a0, 0xEEF     # 하위 12비트 → 부호 확장 주의!
# 0xEEF = 0x800 이상이면 MSB=1 → 부호확장으로 -1 적용됨
# 실제: lui에서 0xDEADC000, addi에서 -0x111 → 결과 0xDEADBEEF
```

> **어셈블러 자동 처리**: `li a0, 0xDEADBEEF` 입력 시  
> 어셈블러가 lui+addi 조합을 자동으로 오프셋 보정

### 메모리 Load (I-type)

```asm
lw    rd, offset(rs1)   # 32비트 word 로드 (부호확장)
lh    rd, offset(rs1)   # 16비트 halfword (부호확장)
lb    rd, offset(rs1)   # 8비트 byte (부호확장)
lhu   rd, offset(rs1)   # 16비트 (제로확장)
lbu   rd, offset(rs1)   # 8비트 (제로확장)
# RV64I 추가:
ld    rd, offset(rs1)   # 64비트 doubleword
lwu   rd, offset(rs1)   # 32비트 (제로확장)
```

### 메모리 Store (S-type)

```asm
sw    rs2, offset(rs1)  # 32비트 저장
sh    rs2, offset(rs1)  # 하위 16비트 저장
sb    rs2, offset(rs1)  # 하위 8비트 저장
# RV64I 추가:
sd    rs2, offset(rs1)  # 64비트 저장
```

### 분기 (B-type) — PC-relative, ±4KB

```asm
beq   rs1, rs2, label   # rs1 == rs2
bne   rs1, rs2, label   # rs1 != rs2
blt   rs1, rs2, label   # rs1 < rs2  (signed)
bge   rs1, rs2, label   # rs1 >= rs2 (signed)
bltu  rs1, rs2, label   # unsigned <
bgeu  rs1, rs2, label   # unsigned >=
```

> **주의**: `bgt`, `ble` 없음 → 어셈블러 pseudo로 rs1/rs2 swap

### 점프 (J/I-type)

```asm
jal   rd, label         # rd = PC+4; PC = PC + offset21  (J-type, ±1MB)
jalr  rd, rs1, imm12    # rd = PC+4; PC = (rs1+imm12) & ~1 (I-type)
```

**일반 사용 패턴:**
```asm
jal   ra, func          # 함수 호출 (ra에 리턴 주소)
jalr  zero, ra, 0       # 함수 리턴 (ret)
jal   zero, label       # 무조건 점프 (j label)
jalr  zero, rs1, 0      # 간접 점프
```

---

## 5. 의사 명령어 (Pseudo Instructions)

어셈블러가 실제 명령어로 변환해주는 편의 명령:

```asm
nop                     → addi x0, x0, 0
mv   rd, rs             → addi rd, rs, 0
li   rd, imm            → lui+addi 또는 addi 조합
la   rd, symbol         → auipc+addi (PIC), lui+addi (절대주소)
neg  rd, rs             → sub rd, x0, rs
not  rd, rs             → xori rd, rs, -1
seqz rd, rs             → sltiu rd, rs, 1
snez rd, rs             → sltu rd, x0, rs
sltz rd, rs             → slt rd, rs, x0
sgtz rd, rs             → slt rd, x0, rs
beqz rs, label          → beq rs, x0, label
bnez rs, label          → bne rs, x0, label
blez rs, label          → bge x0, rs, label
bgez rs, label          → bge rs, x0, label
bltz rs, label          → blt rs, x0, label
bgtz rs, label          → blt x0, rs, label
bgt  rs, rt, label      → blt rt, rs, label
ble  rs, rt, label      → bge rt, rs, label
j    label              → jal x0, label
jr   rs                 → jalr x0, rs, 0
ret                     → jalr x0, ra, 0
call label              → auipc ra, off_hi; jalr ra, ra, off_lo
tail label              → auipc t1, off_hi; jalr x0, t1, off_lo
```

---

## 6. RV64I — 64비트 전용 명령어

```asm
# W suffix = 32비트 연산, 결과를 64비트로 부호확장
addw  rd, rs1, rs2      # (rs1 + rs2)[31:0], 부호확장
subw  rd, rs1, rs2
sllw  rd, rs1, rs2
srlw  rd, rs1, rs2
sraw  rd, rs1, rs2
addiw rd, rs1, imm
slliw rd, rs1, shamt
srliw rd, rs1, shamt
sraiw rd, rs1, shamt
```

> **W 명령어 중요성**: `int` (32비트) 연산 후 오버플로우/확장 동작이  
> 64비트 연산과 다름 → C의 `int` vs `long` 구분에 대응

---

## 7. M 확장 — 곱셈/나눗셈

```asm
# RV32M
mul   rd, rs1, rs2      # rd = (rs1 * rs2)[31:0]   하위 32비트
mulh  rd, rs1, rs2      # rd = (signed*signed)[63:32] 상위 32비트
mulhu rd, rs1, rs2      # rd = (unsigned*unsigned) 상위
mulhsu rd, rs1, rs2     # rd = (signed*unsigned) 상위

div   rd, rs1, rs2      # 부호있는 나눗셈 (0제수 → -1)
divu  rd, rs1, rs2      # 부호없는 나눗셈 (0제수 → 2^32-1)
rem   rd, rs1, rs2      # 나머지 (부호있는, 결과 부호 = 피제수)
remu  rd, rs1, rs2      # 나머지 (부호없는)

# RV64M 추가
mulw  rd, rs1, rs2      # 32비트 곱, 부호확장
divw / divuw / remw / remuw
```

### 64비트 곱셈 패턴 (32비트 기계):
```asm
# uint64_t = uint32_t * uint32_t
mulhu a1, a0, a2        # 상위 32비트
mul   a0, a0, a2        # 하위 32비트
```

### 나눗셈 예외 동작 (x86과 다름!)
| 조건 | 결과 |
|------|------|
| 0으로 나누기 | -1 또는 2^N-1 (예외 없음!) |
| INT_MIN / -1 | INT_MIN (오버플로우, 예외 없음) |

---

## 8. A 확장 — Atomic 명령어

### Load-Reserved / Store-Conditional (LL/SC 패턴)

```asm
lr.w   rd, (rs1)            # Load-Reserved: 예약 설정
sc.w   rd, rs2, (rs1)       # Store-Conditional: 성공=0, 실패=nonzero

# .aq / .rl 순서 지정자
lr.w.aq  rd, (rs1)          # acquire 시맨틱
sc.w.rl  rd, rs2, (rs1)     # release 시맨틱
lr.w.aqrl ...               # 둘 다
```

**CAS (Compare-And-Swap) 구현:**
```asm
# if (*addr == expected) { *addr = new; return 0; } else return 1;
cas:
    lr.w.aq  t0, (a0)       # t0 = *addr, reserve
    bne      t0, a1, fail   # t0 != expected → fail
    sc.w.rl  t1, a2, (a0)   # *addr = new (조건부)
    bnez     t1, cas        # sc 실패 시 재시도
    li       a0, 0
    ret
fail:
    li       a0, 1
    ret
```

### AMO (Atomic Memory Operations)

```asm
amoswap.w  rd, rs2, (rs1)   # rd = *rs1; *rs1 = rs2
amoadd.w   rd, rs2, (rs1)   # rd = *rs1; *rs1 += rs2
amoand.w   rd, rs2, (rs1)
amoor.w    rd, rs2, (rs1)
amoxor.w   rd, rs2, (rs1)
amomin.w   rd, rs2, (rs1)   # signed min
amomax.w   rd, rs2, (rs1)   # signed max
amominu.w  rd, rs2, (rs1)   # unsigned min
amomaxu.w  rd, rs2, (rs1)   # unsigned max

# RV64A: .d suffix (64비트 버전)
amoswap.d  rd, rs2, (rs1)
```

> 모두 `.aq`, `.rl`, `.aqrl` 순서 지정자 붙일 수 있음

---

## 9. 메모리 순서 모델 (Memory Ordering)

### RISC-V 메모리 모델: RVWMO
- **x86보다 약한 모델** (TSO보다 약함)
- 같은 하트(hart) 내 순서도 프로그램 순서 보장 안됨 (speculation)
- 멀티코어에서 Store-Load reordering 가능

### FENCE 명령어

```asm
fence  pred, succ
# pred/succ: i(instruction), r(read), w(write), rw
fence  rw, rw            # 완전한 메모리 배리어
fence  r, r              # load-load 배리어
fence  w, w              # store-store 배리어
fence  r, rw             # load 후 모든 메모리 접근
fence.i                  # instruction-fetch 배리어 (I$ flush)
```

### 메모리 배리어 vs AMO ordering

```asm
# Mutex lock 패턴
lock:
    li       t0, 1
    amoswap.w.aq  t1, t0, (a0)  # acquire 시맨틱 → 이후 접근이 앞으로 안감
    bnez     t1, lock

# Mutex unlock 패턴
    amoswap.w.rl  x0, x0, (a0)  # release 시맨틱 → 이전 접근이 뒤로 안감
```

---

## 10. F/D 확장 — 부동소수점

### 부동소수점 레지스터 (32개)

```
f0  = ft0    임시
f1  = ft1    임시
f2  = ft2    임시
f3  = ft3    임시
f4  = ft4    임시
f5  = ft5    임시
f6  = ft6    임시
f7  = ft7    임시
f8  = fs0    Callee-saved
f9  = fs1    Callee-saved
f10 = fa0    인자/반환값
f11 = fa1    인자/반환값
f12 = fa2    인자
f13 = fa3    인자
f14 = fa4    인자
f15 = fa5    인자
f16 = fa6    인자
f17 = fa7    인자
f18 = fs2    Callee-saved
...
f27 = fs9
f28 = ft8    임시
f29 = ft9
f30 = ft10
f31 = ft11
```

> **NaN boxed**: D(64비트) 레지스터에 F(32비트) 값 저장 시 상위 32비트 = 모두 1

### F 확장 (단정밀도, 32비트)

```asm
# 로드/저장
flw   fd, offset(rs1)
fsw   fs2, offset(rs1)

# 산술
fadd.s  fd, fs1, fs2
fsub.s  fd, fs1, fs2
fmul.s  fd, fs1, fs2
fdiv.s  fd, fs1, fs2
fsqrt.s fd, fs1
fmin.s  fd, fs1, fs2
fmax.s  fd, fs1, fs2
fneg.s  fd, fs1              # 부호 반전 (pseudo: fsgnjn.s fd, fs1, fs1)

# Fused Multiply-Add (정밀도 손실 없이)
fmadd.s  fd, fs1, fs2, fs3   # fd = (fs1*fs2) + fs3
fmsub.s  fd, fs1, fs2, fs3   # fd = (fs1*fs2) - fs3
fnmadd.s fd, fs1, fs2, fs3   # fd = -((fs1*fs2) + fs3)
fnmsub.s fd, fs1, fs2, fs3   # fd = -((fs1*fs2) - fs3)

# 비교 (결과는 정수 레지스터로)
feq.s  rd, fs1, fs2          # rd = (fs1 == fs2)
flt.s  rd, fs1, fs2          # rd = (fs1 < fs2)
fle.s  rd, fs1, fs2          # rd = (fs1 <= fs2)
fclass.s rd, fs1             # 분류 비트마스크 반환

# 변환
fcvt.w.s  rd, fs1, rm        # float → int32
fcvt.wu.s rd, fs1, rm        # float → uint32
fcvt.s.w  fd, rs1, rm        # int32 → float
fcvt.s.wu fd, rs1, rm        # uint32 → float
# RV64F 추가
fcvt.l.s  rd, fs1, rm        # float → int64
fcvt.lu.s rd, fs1, rm
fcvt.s.l  fd, rs1, rm
fcvt.s.lu fd, rs1, rm

# 비트 이동 (정수 ↔ FP)
fmv.x.w  rd, fs1             # FP reg → 정수 reg (비트 그대로)
fmv.w.x  fd, rs1             # 정수 reg → FP reg (비트 그대로)
```

### D 확장 (배정밀도, 64비트)

```asm
fld   fd, offset(rs1)
fsd   fs2, offset(rs1)
fadd.d / fsub.d / fmul.d / fdiv.d / fsqrt.d
fmadd.d / fmsub.d / fnmadd.d / fnmsub.d
feq.d / flt.d / fle.d / fclass.d
fcvt.s.d  fd, fs1            # double → float
fcvt.d.s  fd, fs1            # float → double
fcvt.w.d / fcvt.wu.d / fcvt.l.d / fcvt.lu.d
fcvt.d.w / fcvt.d.wu / fcvt.d.l / fcvt.d.lu
fmv.x.d  rd, fs1             # RV64D only
fmv.d.x  fd, rs1             # RV64D only
```

### 반올림 모드 (rm 필드, 3비트)

| 값 | 이름 | 설명 |
|----|------|------|
| 000 | RNE | Round to Nearest Even (기본) |
| 001 | RTZ | Round Towards Zero |
| 010 | RDN | Round Down (−∞ 방향) |
| 011 | RUP | Round Up (+∞ 방향) |
| 100 | RMM | Round to Nearest, ties to Max Magnitude |
| 111 | DYN | fcsr.frm 레지스터 값 사용 |

### FCSR (Floating-Point Control and Status Register)

```
Bit[7:5] = frm   반올림 모드
Bit[4]   = NV    Invalid Operation
Bit[3]   = DZ    Divide by Zero
Bit[2]   = OF    Overflow
Bit[1]   = UF    Underflow
Bit[0]   = NX    Inexact

frcsr  rd             # fcsr 읽기
fscsr  rs1            # fcsr 쓰기
frrm   rd             # frm 읽기
fsrm   rs1            # frm 쓰기
frflags rd            # 예외 플래그 읽기
fsflags rs1           # 예외 플래그 쓰기
```

### fclass 결과 비트마스크

```
Bit 0: −∞
Bit 1: negative normal
Bit 2: negative subnormal
Bit 3: −0
Bit 4: +0
Bit 5: positive subnormal
Bit 6: positive normal
Bit 7: +∞
Bit 8: signaling NaN
Bit 9: quiet NaN
```

---

## 11. V 확장 — 벡터 (SIMD)

### 벡터 레지스터 (v0–v31)
- **가변 길이**: 하드웨어가 VLEN 비트 폭 구현 (128~65536비트)
- **SEW**: Standard Element Width (8/16/32/64비트)
- **LMUL**: Length Multiplier (레지스터 그루핑, 1/2/4/8 또는 1/2/1/4/1/8)

### 벡터 구성 명령어

```asm
vsetvli  rd, rs1, vtypei    # vl = min(rs1, vlmax) 설정, vtype 설정
vsetivli rd, uimm5, vtypei  # rs1 대신 5비트 즉치
vsetvl   rd, rs1, rs2       # vtype을 레지스터로 지정

# vtypei 인코딩
# vsew[2:0]: 000=8, 001=16, 010=32, 011=64
# vlmul[2:0]: 000=1, 001=2, 010=4, 011=8, 101=1/8, 110=1/4, 111=1/2
# vta: tail agnostic
# vma: mask agnostic

# 예시: SEW=32, LMUL=4
vsetvli t0, a0, e32, m4, ta, ma
```

### 벡터 로드/스토어

```asm
# Unit-stride
vle8.v  / vle16.v / vle32.v / vle64.v   vd, (rs1)
vse8.v  / vse16.v / vse32.v / vse64.v   vs3, (rs1)

# Strided
vlse32.v  vd, (rs1), rs2     # rs2 = stride (바이트 단위)
vsse32.v  vs3, (rs1), rs2

# Indexed (gather/scatter)
vluxei32.v  vd, (rs1), vs2   # unordered
vloxei32.v  vd, (rs1), vs2   # ordered
vsuxei32.v  vs3, (rs1), vs2
vsoxei32.v  vs3, (rs1), vs2

# Whole register
vl1r.v  vd, (rs1)
vs1r.v  vs3, (rs1)
```

### 벡터 산술

```asm
# 정수 연산
vadd.vv   vd, vs2, vs1
vadd.vx   vd, vs2, rs1       # vs2[i] + rs1 (scalar broadcast)
vadd.vi   vd, vs2, imm5
vsub.vv   vd, vs2, vs1
vmul.vv   vd, vs2, vs1
vmulh.vv  vd, vs2, vs1       # 곱셈 상위 절반 (signed)
vmulhu.vv / vmulhsu.vv
vdiv.vv   vd, vs2, vs1
vremu.vv  vd, vs2, vs1
vsll.vv / vsrl.vv / vsra.vv
vand.vv / vor.vv / vxor.vv

# 부동소수점
vfadd.vv   vd, vs2, vs1
vfsub.vv   vd, vs2, vs1
vfmul.vv   vd, vs2, vs1
vfdiv.vv   vd, vs2, vs1
vfsqrt.v   vd, vs2
vfmadd.vv  vd, vs1, vs2      # vd[i] = vs1[i]*vd[i] + vs2[i]
vfnmadd.vv / vfmsub.vv / vfnmsub.vv
vfmin.vv / vfmax.vv
```

### 벡터 마스크

```asm
# v0 레지스터가 마스크 레지스터
vadd.vv  vd, vs2, vs1, v0.t  # v0.t = tail mask
vmseq.vv  v0, vs2, vs1        # 마스크 생성: vs2[i] == vs1[i]
vmslt.vv  v0, vs2, vs1
vmand.mm  vd, vs1, vs2        # 마스크 AND
vmor.mm   vd, vs1, vs2
vmxor.mm  vd, vs1, vs2
vmnot.m   vd, vs2
vcpop.m   rd, vs2             # 마스크에서 1 개수 세기
```

### 벡터 리덕션

```asm
vredsum.vs  vd, vs2, vs1      # vd[0] = sum(vs2) + vs1[0]
vredmax.vs  vd, vs2, vs1
vredmin.vs  vd, vs2, vs1
vredand.vs / vredor.vs / vredxor.vs
vfredusum.vs / vfredosum.vs   # 부동소수점 리덕션
```

### SIMD 사용 패턴 (벡터화된 SAXPY)

```asm
# void saxpy(float a, float* x, float* y, int n)
# y[i] = a * x[i] + y[i]
saxpy:
    vsetvli   t0, a3, e32, m8, ta, ma   # SEW=32, LMUL=8
loop:
    vle32.v   v0, (a1)          # x 로드
    vle32.v   v8, (a2)          # y 로드
    vfmadd.vf v0, fa0, v8       # v0[i] = fa0 * v0[i] + v8[i]
    vse32.v   v0, (a2)          # y 저장
    sub       a3, a3, t0        # 남은 원소 수 감소
    slli      t1, t0, 2         # t1 = t0 * 4 (바이트)
    add       a1, a1, t1
    add       a2, a2, t1
    vsetvli   t0, a3, e32, m8, ta, ma   # 남은 원소로 vl 재설정
    bnez      a3, loop
    ret
```

---

## 12. Zicsr 확장 — CSR 명령어

### CSR 명령어

```asm
csrrw   rd, csr, rs1    # rd = csr; csr = rs1  (Read-Write)
csrrs   rd, csr, rs1    # rd = csr; csr |= rs1  (Set bits)
csrrc   rd, csr, rs1    # rd = csr; csr &= ~rs1 (Clear bits)
csrrwi  rd, csr, uimm5  # 즉치 버전
csrrsi  rd, csr, uimm5
csrrci  rd, csr, uimm5

# Pseudo: rd = x0이면 결과 버림
csrr    rd, csr         → csrrs rd, csr, x0
csrw    csr, rs1        → csrrw x0, csr, rs1
csrs    csr, rs1        → csrrs x0, csr, rs1
csrc    csr, rs1        → csrrc x0, csr, rs1
```

### 주요 CSR 목록

```
# Machine-level
mstatus   0x300   Machine status (전역 인터럽트 허용 등)
misa      0x301   구현된 ISA 확장 비트맵
medeleg   0x302   예외 위임 (to S-mode)
mideleg   0x303   인터럽트 위임
mie       0x304   Machine interrupt enable
mtvec     0x305   트랩 벡터 베이스 주소
mscratch  0x340   M-mode 스크래치 레지스터
mepc      0x341   Machine exception PC
mcause    0x342   트랩 원인
mtval     0x343   트랩 값 (폴트 주소 등)
mip       0x344   Machine interrupt pending

# Supervisor-level
sstatus   0x100
sie       0x104
stvec     0x105
sscratch  0x140
sepc      0x141
scause    0x142
stval     0x143
sip       0x144
satp      0x180   페이지 테이블 베이스 (Sv39/Sv48)

# Counter
cycle     0xC00   사이클 카운터 (rdcycle)
time      0xC01   타이머 (rdtime)
instret   0xC02   실행 명령어 수 (rdinstret)

# Floating-point
fcsr      0x003
frm       0x002
fflags    0x001
```

### mstatus 비트 구조

```
Bit[3]  = MIE   M-mode 전역 인터럽트 허용
Bit[1]  = SIE   S-mode 전역 인터럽트 허용
Bit[7]  = MPIE  이전 MIE 값 (트랩 시 저장)
Bit[5]  = SPIE  이전 SIE 값
Bit[12:11] = MPP   이전 특권 수준 (M=3, S=1, U=0)
Bit[8]  = SPP   이전 특권 수준 (S=1, U=0)
Bit[14:13] = FS   FPU 상태 (Off/Initial/Clean/Dirty)
Bit[16:15] = XS   확장 상태
Bit[17] = MPRV  메모리 접근 특권 오버라이드
Bit[18] = SUM   S-mode의 U-mode 페이지 접근 허용
Bit[19] = MXR   Executable page 읽기 허용
```

---

## 13. 특권 수준 (Privilege Levels)

```
Ring 3: User Mode (U-mode)
Ring 1: Supervisor Mode (S-mode) — OS 커널
Ring 0: Machine Mode (M-mode) — 펌웨어/SBI
(선택적) Hypervisor (H-mode) — 가상화
```

### 특권 전환

```asm
# U → M/S: 예외/인터럽트 발생 시 자동
# M → U/S: mret 명령어
mret        # PC = mepc, 특권 = MPP, MIE = MPIE
sret        # PC = sepc, 특권 = SPP, SIE = SPIE
# S → M: ecall (or 예외)
ecall       # 환경 호출 (시스템콜)
ebreak      # 디버거 브레이크포인트
wfi         # Wait For Interrupt (저전력 대기)
```

### 트랩 처리 흐름

```
1. 예외/인터럽트 발생
2. mepc/sepc ← PC
3. mcause/scause ← 원인
4. mtval/stval ← 추가 정보 (폴트 주소 등)
5. mstatus.MPIE ← MIE; MIE ← 0 (인터럽트 비활성화)
6. mstatus.MPP ← 현재 특권
7. PC ← mtvec (직접 모드) 또는 mtvec + 4*cause (벡터 모드)
```

### mcause 주요 값

```
# 인터럽트 (bit[XLEN-1]=1)
1  = Supervisor software interrupt
3  = Machine software interrupt
5  = Supervisor timer interrupt
7  = Machine timer interrupt
9  = Supervisor external interrupt
11 = Machine external interrupt

# 예외 (bit[XLEN-1]=0)
0  = Instruction address misaligned
1  = Instruction access fault
2  = Illegal instruction
3  = Breakpoint (ebreak)
4  = Load address misaligned
5  = Load access fault
6  = Store/AMO address misaligned
7  = Store/AMO access fault
8  = Environment call from U-mode
9  = Environment call from S-mode
11 = Environment call from M-mode
12 = Instruction page fault
13 = Load page fault
15 = Store/AMO page fault
```

---

## 14. 가상 메모리 (Sv39/Sv48)

### satp CSR 구조

```
RV64 satp:
Bit[63:60] = MODE  (0=Bare, 8=Sv39, 9=Sv48, 10=Sv57)
Bit[59:44] = ASID  (Address Space ID, 16비트)
Bit[43:0]  = PPN   (Physical Page Number of root page table)
```

### Sv39 — 39비트 가상 주소

```
VA[38:30] = VPN[2]  (512GB 단위)
VA[29:21] = VPN[1]  (1GB 단위)
VA[20:12] = VPN[0]  (2MB 단위)
VA[11:0]  = Page offset (4KB)

# 3단계 페이지 테이블 워크
# PTE (Page Table Entry, 64비트):
Bit[63:54] = Reserved
Bit[53:10] = PPN  (Physical Page Number)
Bit[9:8]   = RSW  (소프트웨어용)
Bit[7]     = D    (Dirty)
Bit[6]     = A    (Accessed)
Bit[5]     = G    (Global)
Bit[4]     = U    (User-mode 접근 가능)
Bit[3]     = X    (Executable)
Bit[2]     = W    (Writable)
Bit[1]     = R    (Readable)
Bit[0]     = V    (Valid)

# Leaf PTE: R|W|X 중 하나 이상 set
# Non-leaf PTE: R=W=X=0, PPN은 다음 레벨 PT
```

### TLB 관련

```asm
sfence.vma  rs1, rs2    # TLB 플러시
# rs1=x0: 모든 주소
# rs2=x0: 모든 ASID
# 둘 다 x0: 전체 TLB flush
sfence.vma  a0, x0      # 특정 VA에 대한 TLB flush
sfence.vma  a0, a1      # 특정 VA + ASID
```

---

## 15. C 확장 — 압축 명령어 (16비트)

```asm
# 스택 상대 로드/저장
c.lwsp  rd, offset      # lw rd, offset(sp)  [rd ≠ x0]
c.swsp  rs2, offset     # sw rs2, offset(sp)
c.ldsp  rd, offset      # ld (RV64)
c.sdsp  rs2, offset

# 레지스터 기반 (8개 레지스터만: x8-x15 = s0-a5)
c.lw    rd', offset(rs1')
c.sw    rs2', offset(rs1')
c.ld    rd', offset(rs1')   # RV64C
c.sd    rs2', offset(rs1')

# 산술 (제한된 레지스터)
c.add   rd, rs2         # rd += rs2
c.sub   rd', rs2'       # rd' -= rs2'
c.and   rd', rs2'
c.or    rd', rs2'
c.xor   rd', rs2'
c.mv    rd, rs2         # rd = rs2
c.li    rd, imm6        # rd = sign_ext(imm6)
c.lui   rd, imm6        # rd = imm6 << 12
c.addi  rd, imm6        # rd += sign_ext(imm6)
c.addiw rd, imm6        # (RV64C) 32비트 addi
c.addi16sp  x2, imm     # sp += sign_ext(imm*16)
c.addi4spn  rd', imm    # rd' = sp + uimm*4
c.slli  rd, shamt
c.srli  rd', shamt
c.srai  rd', shamt
c.andi  rd', imm6

# 분기/점프
c.j     label           # 무조건 점프 (jal x0)
c.jal   label           # 점프+링크 (RV32C만, jal ra) 
c.jr    rs1             # jalr x0, rs1, 0
c.jalr  rs1             # jalr ra, rs1, 0
c.beqz  rs1', label     # beq rs1', x0
c.bnez  rs1', label     # bne rs1', x0

# 특수
c.nop                   # addi x0, x0, 0
c.ebreak
```

---

## 16. 함수 호출 규약 (ABI) 심화

### 스택 프레임 구조

```
고주소
┌───────────────────┐ ← 이전 sp (호출 전)
│    인자 8개 이후   │   (7번째+ 인자 역순으로)
├───────────────────┤
│       ra          │ ← callee가 저장
│       s0/fp       │
│       s1 ~ s11    │ (사용한 것만)
├───────────────────┤ ← fp (frame pointer, optional)
│     로컬 변수      │
├───────────────────┤ ← sp (현재)
저주소
```

### Prologue / Epilogue 패턴

```asm
func:
    # Prologue
    addi  sp, sp, -32        # 스택 할당
    sw    ra, 28(sp)         # ra 저장
    sw    s0, 24(sp)         # fp 저장
    addi  s0, sp, 32         # fp = old sp

    # ... 함수 본체 ...

    # Epilogue
    lw    ra, 28(sp)
    lw    s0, 24(sp)
    addi  sp, sp, 32
    ret                      # jalr x0, ra, 0
```

### Variadic 함수 (va_list)

```c
// RV64 ABI: va_list = void* (stack argument pointer)
// 8개(a0-a7) 소진 후 스택에서 순서대로
// int 인자도 a0-a7에서 레지스터 크기로 전달 (zero/sign-extended to XLEN)
```

### 반환값 규칙

| 타입 | 반환 레지스터 |
|------|--------------|
| 32비트 이하 정수 | a0 |
| 64비트 정수 (RV32) | a0(하위), a1(상위) |
| 64비트 정수 (RV64) | a0 |
| float (F확장) | fa0 |
| double (D확장) | fa0 |
| 구조체 ≤ 2×XLEN | a0, a1 |
| 큰 구조체 | a0 = 포인터 (숨겨진 첫 인자) |

---

## 17. Zifencei / Zicntr / Zihpm

```asm
# Zifencei
fence.i             # I-cache 동기화 (JIT, self-modifying code 후 필수)

# Zicntr (카운터)
rdcycle   rd        # 사이클 카운터 읽기 (= csrr rd, cycle)
rdtime    rd        # 실시간 타이머 (= csrr rd, time)
rdinstret rd        # 실행된 명령어 수
# RV32 전용:
rdcycleh  rd        # cycle 상위 32비트
rdtimeh   rd
rdinstreth rd

# Zihpm (성능 카운터)
hpmcounter3~31      # CSR 0xC03~0xC1F
hpmcounterh3~31     # RV32 상위 32비트
```

---

## 18. 링커 & ABI 수준 개념

### Relocation 타입

```
R_RISCV_32          절대 32비트 참조
R_RISCV_64          절대 64비트 참조
R_RISCV_PCREL_HI20  auipc의 상위 20비트 (PC-relative)
R_RISCV_PCREL_LO12_I jalr/load의 하위 12비트
R_RISCV_PCREL_LO12_S store의 하위 12비트
R_RISCV_HI20        lui의 상위 20비트 (절대)
R_RISCV_LO12_I      하위 12비트 I-type
R_RISCV_LO12_S      하위 12비트 S-type
R_RISCV_CALL        auipc+jalr 쌍 (call)
R_RISCV_JAL         jal의 21비트 오프���
R_RISCV_BRANCH      분기 13비트 오프셋
```

### 코드 모델 (Code Model)

| 모델 | 범위 | 설명 |
|------|------|------|
| medlow | ±2GB | lui+addi (절대 주소) |
| medany | 임의 | auipc+addi (PC-relative, 기본값) |
| large | 전체 | lui+addi+shift 조합 (RV64) |

### GOT / PLT (동적 링킹)

```asm
# GOT: Global Offset Table
# 컴파일러가 auipc + load 패턴으로 GOT 엔트리 접근
auipc  t0, %got_pcrel_hi(sym)
ld     t0, %pcrel_lo(1b)(t0)   # GOT에서 실제 주소 로드

# PLT: Procedure Linkage Table (lazy binding)
auipc  t1, %pcrel_hi(.got.plt+idx)
l[w|d] t0, %pcrel_lo(1b)(t1)
jr     t0
```

---

## 19. 인터럽트 & 예외 처리 (실제 구현)

### mtvec 모드

```
mtvec[1:0] = 0: Direct   모든 트랩 → BASE
mtvec[1:0] = 1: Vectored 인터럽트 → BASE + 4*cause
                          예외는 항상 BASE
```

### 최소 트랩 핸들러 (M-mode)

```asm
.align 4
trap_handler:
    # 컨텍스트 저장 (mscratch 활용)
    csrrw   sp, mscratch, sp    # sp ↔ mscratch (커널 스택으로 전환)
    addi    sp, sp, -128
    sw      ra,   0(sp)
    sw      t0,   4(sp)
    # ... 모든 caller-saved 레지스터 저장 ...
    sw      a0,  40(sp)

    csrr    a0, mcause
    csrr    a1, mepc
    csrr    a2, mtval

    # mcause[31] = 1: 인터럽트, 0: 예외
    bltz    a0, interrupt_handler
    j       exception_handler

trap_return:
    lw      ra,   0(sp)
    # ... 복원 ...
    lw      a0,  40(sp)
    addi    sp, sp, 128
    csrrw   sp, mscratch, sp    # 스택 복구
    mret                         # 복귀
```

### SBI (Supervisor Binary Interface)

```asm
# S-mode에서 M-mode 서비스 호출 (ecall 사용)
# a7 = extension ID, a6 = function ID
# 반환: a0 = error code, a1 = value

# 콘솔 출력 (Legacy SBI)
li      a7, 1           # SBI_CONSOLE_PUTCHAR
li      a0, 'H'
ecall

# 타이머 설정 (SBI Timer Extension, EID=0x54494D45)
li      a7, 0x54494D45  # "TIME"
li      a6, 0           # sbi_set_timer
mv      a0, s0          # deadline
ecall
```

---

## 20. 성능 최적화 & 파이프라인 상식

### 데이터 해저드 (Data Hazard)

```asm
# RAW (Read After Write) — 1~2사이클 스톨 가능
add  a0, a1, a2      # a0 쓰기
add  a3, a0, a4      # a0 읽기 → forwarding으로 보통 해결됨

# Load-Use 해저드 — 반드시 1사이클 버블
lw   a0, 0(sp)       # 메모리에서 a0 로드
add  a1, a0, a2      # a0 사용 → 1 stall (forwarding 불가)
# 최적화: 중간에 무관한 명령어 삽입
lw   a0, 0(sp)
add  a2, a3, a4      # 무관한 명령어 삽입
add  a1, a0, a2      # stall 없음
```

### 분기 예측 & 파이프라인

```asm
# RISC-V: 분기 예측 실패 시 파이프라인 플러시
# 일반적으로 2~3 사이클 패널티

# 분기 없는 코드 패턴 (branchless)
# if (a0 > 0) a0 = 1; else a0 = -1;
slt   t0, x0, a0     # t0 = (0 < a0)
slt   t1, a0, x0     # t1 = (a0 < 0)
sub   a0, t0, t1     # a0 = t0 - t1
```

### 메모리 접근 정렬 (Alignment)

```
RISC-V 기본: 비정렬 접근 허용 (하드웨어에 따라 트랩 or 느림)
- 일부 구현: 비정렬 시 misaligned exception → 소프트웨어 에뮬레이션
- 고성능 코어: 하드웨어에서 비정렬 처리 (성능 저하 있음)
- AMO/LR/SC: 항상 정렬 필요 (비정렬 → 예외)
```

### 캐시 관련

```asm
# RISC-V 명세: 캐시 관리 명령어 표준 없음 (Zicbom 확장)
# Zicbom (Cache Block Management):
cbo.clean  0(rs1)    # 캐시 블록 clean (write back)
cbo.flush  0(rs1)    # clean + invalidate
cbo.inval  0(rs1)    # invalidate (DMA 수신 전)
# Zicbop (Prefetch):
prefetch.r 0(rs1)    # 읽기 프리페치
prefetch.w 0(rs1)    # 쓰기 프리페치
prefetch.i 0(rs1)    # 명령어 프리페치
```

---

## 21. ELF & 실행 파일 구조

```
# RISC-V ELF 식별
e_machine = EM_RISCV (0xF3)

# EF_RISCV_* 플래그 (e_flags)
EF_RISCV_RVC        = 0x0001  C 확장 사용
EF_RISCV_FLOAT_ABI_SOFT   = 0x0000
EF_RISCV_FLOAT_ABI_SINGLE = 0x0002  F 확장
EF_RISCV_FLOAT_ABI_DOUBLE = 0x0004  D 확장
EF_RISCV_FLOAT_ABI_QUAD   = 0x0006  Q 확장
EF_RISCV_RVE        = 0x0008  RV32E
EF_RISCV_TSO        = 0x0010  TSO 메모리 모델

# 표준 섹션
.text   실행 코드
.rodata 읽기 전용 데이터
.data   초기화된 전역 변수
.bss    초기화 안 된 전역 변수 (파일에 공간 없음)
.sdata  Small Data (gp 기준 ±2KB, 빠른 접근)
.sbss   Small BSS
```

### GP (Global Pointer) 최적화

```asm
# gp(x3)를 .sdata 중간에 맞추면
# gp-relative 접근으로 lui 없이 12비트 offset만으로 전역변수 접근
# 링커 스크립트에서:
# __global_pointer$ = . + 0x800;

# 컴파일러 생성 코드:
lw  a0, var@gprel(gp)     # lui+lw 대신 lw 하나로
```

---

## 22. 디버그 & 트레이스 (RISC-V Debug Spec)

```asm
ebreak                  # 디버거 브레이크포인트
# dcsr (Debug CSR) 설정:
# dcsr.cause: 트리거 원인 (ebreak/step/haltreq 등)
# dcsr.step: 단계 실행 모드
# dpc: Debug PC

# 트리거 모듈 (하드웨어 브레이크포인트)
tselect  tdata1  tdata2  # 트리거 선택 및 설정
# tdata1.type = 2: address/data match
# mcontrol: 주소/데이터 감시점
```

---

## 요약 치트시트

```
기본 명령어 카운트:
  RV32I:  47개
  +M:     8개 추가
  +A:     11개 추가
  +F:     ~26개 추가
  +D:     ~26개 추가
  +C:     ~36개 추가
  +V:     ~350+개 추가

자주 틀리는 포인트:
  ✗ RISC-V는 zero-flag 없음 → beqz는 pseudo
  ✗ jal 범위는 ±1MB (21비트), bXX는 ±4KB (13비트)
  ✗ lui의 imm은 31:12 비트 → addi 상위 보정 필요
  ✗ 나눗셈 0으로는 예외 없이 -1 반환
  ✗ lr/sc/amo는 주소 정렬 필수
  ✗ fence.i는 I$ flush, fence rw,rw는 데이터 배리어
  ✗ mret/sret 전에 반드시 mepc/sepc, mstatus 설정
  ✗ 부동소수점 DYN 반올림은 fcsr.frm 참조
  ✗ V확장: vsetvli 결과(vl)는 요청값과 다를 수 있음
```

---