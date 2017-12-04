#define ROM_NO 3

//#define INS_MOV 1
#define INS_LOD 1
//#define INS_STO 1
//#define INS_ALU 1

//#define DUMP 1

//#define CHECK_COVERAGE 1


#include <MyEEPROM.h>
#include <stdint.h>


/* signal word: bit position  31     30     29     28     27     26     25     24     23     22     21     20     19     18     17     16                 
 *              meaning       _TR    -      -      -      -      -      _ME    _MW    _HLT   PGM    _MAW   _IRW   _RdE   _RdW   _RcE   _RcW
 *              bit position  15     14     13     12     11     10     09     08     07     06     05     04     03     02     01     00
 *              meaning       _RbE   _RbW   _RaE   _RaW   _PCE   _PCW   _SPE   _SPW   PCC    _ALE   _ALW   _ALB   ALS2   ALS1   ALS0   ALC
 */


#define ALC  ((uint32_t)1 << 0)
#define ALS0 ((uint32_t)1 << 1)
#define ALS1 ((uint32_t)1 << 2)
#define ALS2 ((uint32_t)1 << 3)
#define _ALB ((uint32_t)1 << 4)
#define _ALW ((uint32_t)1 << 5)
#define _ALE ((uint32_t)1 << 6)
#define PCC  ((uint32_t)1 << 7)
#define _SPW ((uint32_t)1 << 8)
#define _SPE ((uint32_t)1 << 9)
#define _PCW ((uint32_t)1 << 10)
#define _PCE ((uint32_t)1 << 11)
#define _RaW ((uint32_t)1 << 12)
#define _RaE ((uint32_t)1 << 13)
#define _RbW ((uint32_t)1 << 14)
#define _RbE ((uint32_t)1 << 15)

#define _RcW ((uint32_t)1 << 16)
#define _RcE ((uint32_t)1 << 17)
#define _RdW ((uint32_t)1 << 18)
#define _RdE ((uint32_t)1 << 19)
#define _IRW ((uint32_t)1 << 20)
#define _MAW ((uint32_t)1 << 21)
#define PGM  ((uint32_t)1 << 22)
#define _HLT ((uint32_t)1 << 23)
#define _MW  ((uint32_t)1 << 24)
#define _ME  ((uint32_t)1 << 25)

#define _TR  ((uint32_t)1 << 31)

#define ALS(S) ((uint32_t)S << 1)

#define B_MINUS_A 0b001
#define A_MINUS_B 0b010
#define A_PLUS_B  0b011
#define A_XOR_B   0b100
#define A_OR_B    0b101
#define A_AND_B   0b110


#define INC_A     0b000
#define NOT_A     0b111



/* EEPROM address:  bit position     12       11      10  ....   8      7   .....    0
 *                  meaning       flags    carry     (microtime) T              opcode
 */


#define CARRY_ADDR (1 << 11)
#define FLAGS_ADDR (1 << 12)

#define ins_eeprom_address(opcode, carry, flags, T) (opcode | (carry ? CARRY_ADDR : 0) | (flags ? FLAGS_ADDR : 0) | (T << 8))


#define MOV 0b00
#define LOD 0b01
#define STO 0b10


#define Ra  0b000
#define Rb  0b001
#define Rc  0b010
#define Rd  0b011
#define SP  0b100
#define PC  0b101
#define SPi 0b110
#define IMM 0b111


#define OPCODE(op,dreg,sreg) (((op) << 6) | ((dreg) << 3) | (sreg))
#define ALU_OPCODE(with_carry,S,reg) ((0b11 << 6) | (with_carry ? 1 << 5 : 0) | ((S & 0b111) << 2) | (reg & 0b11))


#ifdef CHECK_COVERAGE
uint8_t write_mask[32];
#endif

uint32_t inline flip_active_lows(uint32_t microcode_word) {

  microcode_word ^= (_ALB | _ALW | _ALE | _SPW | _SPE | _PCW | _PCE | _RaW | _RaE | _RbW | _RbE | _RcW | _RcE | _RdW | _RdE | _MAW | _IRW | _HLT | _MW | _ME | _TR);
  return microcode_word;
}

uint32_t _E(uint32_t reg) {

  switch (reg) {
    case Ra: return _RaE;
    case Rb: return _RbE;
    case Rc: return _RcE;
    case Rd: return _RdE;
    case SP: return _SPE;
    case PC: return _PCE;
    default: return 0; 
  }
}

uint32_t _W(uint8_t reg) {

  switch (reg) {
    case Ra: return _RaW;
    case Rb: return _RbW;
    case Rc: return _RcW;
    case Rd: return _RdW;
    case SP: return _SPW;
    case PC: return _PCW;
    default: return 0;
  }
}

#define FETCH0 (_PCE | _MAW)
#define FETCH1 (PGM | _ME | _IRW | PCC)

uint32_t microcode[8] = {FETCH0,FETCH1,_TR,0,0,0,0,0};


uint32_t *MICROCODE0() {
  microcode[2] = _TR;
  microcode[3] = microcode[4] = microcode[5] = microcode[6] = microcode[7] = 0;
  return microcode;
}

uint32_t *MICROCODE1(uint32_t c1) {
  microcode[2] = c1 | _TR;
  microcode[3] = microcode[4] = microcode[5] = microcode[6] = microcode[7] = 0;
  return microcode;
}

uint32_t *MICROCODE2(uint32_t c1, uint32_t c2) {
  microcode[2] = c1;
  microcode[3] = c2 | _TR;
  microcode[4] = microcode[5] = microcode[6] = microcode[7] = 0;
  return microcode;
}

uint32_t *MICROCODE3(uint32_t c1,uint32_t c2,uint32_t c3) {
  microcode[2] = c1;
  microcode[3] = c2;
  microcode[4] = c3 | _TR;
  microcode[5] = microcode[6] = microcode[7] = 0;
  return microcode;
}

uint32_t *MICROCODE4(uint32_t c1,uint32_t c2,uint32_t c3, uint32_t c4) {
  microcode[2] = c1;
  microcode[3] = c2;
  microcode[4] = c3;
  microcode[5] = c4 | _TR;
  microcode[6] = microcode[7] = 0;
  return microcode;
}

uint32_t *MICROCODE5(uint32_t c1,uint32_t c2,uint32_t c3, uint32_t c4, uint32_t c5) {
  microcode[2] = c1;
  microcode[3] = c2;
  microcode[4] = c3;
  microcode[5] = c4;
  microcode[6] = c5 | _TR;
  microcode[7] = 0;
  return microcode;
}


void write_conditional_instruction(uint16_t opcode, bool carry, bool flags, uint32_t microcode[], uint8_t rom_no) {

  for (uint8_t T = 0; T < 8; T++) {

    uint16_t eeprom_address = ins_eeprom_address(opcode, carry, flags, T);
    uint8_t eeprom_byte = (flip_active_lows(microcode[T]) >> (8*rom_no)) & 0xFF;
    MyEEPROM.write_eeprom(eeprom_address, eeprom_byte);
    
    #ifdef CHECK_COVERAGE
    uint8_t bit_no = eeprom_address & 0x7;
    write_mask[(eeprom_address & 0xFF) >> 3] |= (1 << bit_no);
    #endif
  }
}

void write_carrycond_instruction(uint16_t opcode, bool carry, uint32_t microcode[], uint8_t rom_no) {

  write_conditional_instruction(opcode, carry, false, microcode, rom_no);
  write_conditional_instruction(opcode, carry, true, microcode, rom_no);
}

void write_instruction(uint16_t opcode, uint32_t microcode[], uint8_t rom_no) {

  write_carrycond_instruction(opcode, false, microcode, rom_no);
  write_carrycond_instruction(opcode, true, microcode, rom_no);
}

#ifdef INS_MOV 
void write_MOVs(uint8_t rom_no) {

  Serial.print("Writing reg <- reg MOV instructions ");
  for (uint8_t sreg = Ra; sreg <= PC; sreg++) {

    Serial.print(".");
    for (uint8_t dreg = Ra; dreg <= PC; dreg++) {

      if (sreg != dreg) {

        write_instruction(OPCODE(MOV, dreg, sreg), MICROCODE1(_W(dreg) | _E(sreg)), rom_no);
      }
    }
  }
  Serial.println(" done.");

  Serial.print("Writing reg <- imm DATA instructions .");
  for (uint8_t dreg = Ra; dreg <= PC; dreg++) {

    write_instruction(OPCODE(MOV, dreg, IMM), MICROCODE2(_PCE | _MAW | PCC, PGM | _ME | _W(dreg)), rom_no);
  }
  Serial.println(". done.");

  Serial.print("Writing NOP and HLT .");
  write_instruction(OPCODE(MOV, Ra, Ra), MICROCODE0(), rom_no);
  write_instruction(OPCODE(MOV, PC, PC), MICROCODE1(_HLT), rom_no);
  Serial.println(". done.");

  Serial.print("Writing conditional JC, JZ, JO and JN instructions ");
  write_carrycond_instruction(OPCODE(MOV, IMM, 0b000), false, MICROCODE1(PCC), rom_no);
  write_carrycond_instruction(OPCODE(MOV, IMM, 0b000), true, MICROCODE2(_MAW | _PCE | PCC, PGM | _ME | _PCW), rom_no);
  Serial.print(".");
  for (uint8_t flag = 0; flag <= 2; flag++) {
    write_conditional_instruction(OPCODE(MOV, IMM, 1<<flag), false, false, MICROCODE1(PCC), rom_no);
    write_conditional_instruction(OPCODE(MOV, IMM, 1<<flag), true, false, MICROCODE1(PCC), rom_no);
    write_conditional_instruction(OPCODE(MOV, IMM, 1<<flag), false, true, MICROCODE2(_MAW | _PCE | PCC, PGM | _ME | _PCW), rom_no);
    write_conditional_instruction(OPCODE(MOV, IMM, 1<<flag), true, true, MICROCODE2(_MAW | _PCE | PCC, PGM | _ME | _PCW), rom_no);
    Serial.print(".");
  }
  Serial.println(". done.");
  
  Serial.println("42 MOV instructions written.");
  Serial.print("Writing HLT to currently unused opcodes (22 total): MOV IMM, Rd|PC|SPi|IMM (4)  |  MOV SPi, <any_R>|SPi|IMM (8) | MOV <any_R>, SPi (6) | MOV <any_R>, <thesame_R> (except Ra, PC) (4) ");

  write_instruction(OPCODE(MOV, IMM, Rd), MICROCODE1(_HLT), rom_no);
  write_instruction(OPCODE(MOV, IMM, PC), MICROCODE1(_HLT), rom_no);
  write_instruction(OPCODE(MOV, IMM, SPi), MICROCODE1(_HLT), rom_no);
  write_instruction(OPCODE(MOV, IMM, IMM), MICROCODE1(_HLT), rom_no);
  Serial.print(".");
    
  for (uint8_t sreg = Ra; sreg <= IMM; sreg++) {

    write_instruction(OPCODE(MOV, SPi, sreg), MICROCODE1(_HLT), rom_no);
  }
  Serial.print(".");

  for (uint8_t dreg = Ra; dreg <= PC; dreg++) {

    write_instruction(OPCODE(MOV, dreg, SPi), MICROCODE1(_HLT), rom_no);
  }
  Serial.print(".");

  for (uint8_t reg = Rb; reg <= SP; reg++) {

    write_instruction(OPCODE(MOV, reg, reg), MICROCODE1(_HLT), rom_no);    
  }
  Serial.println(". done.");
}
#endif

#ifdef INS_LOD
void write_LODs(uint8_t rom_no) {

  Serial.print("Writing reg <- [reg] LOD instructions ");
  for (uint8_t sreg = Ra; sreg <= PC; sreg++) {

    Serial.print(".");
    for (uint8_t dreg = Ra; dreg <= PC; dreg++) {

      write_instruction(OPCODE(LOD, dreg, sreg), MICROCODE2(_MAW | _E(sreg), _ME | _W(dreg)), rom_no);
    }
  }
  Serial.println(" done.");

  Serial.print("Writing reg <- [SPi] POP instructions .");
  for (uint8_t dreg = Ra; dreg <= PC; dreg++) {

    write_instruction(OPCODE(LOD, dreg, SPi), MICROCODE3(_SPE | _ALW | ALC | ALS(A_PLUS_B) | _MAW,  _SPW | _ALE, _W(dreg) | _ME), rom_no); 
  }
  Serial.println(". done.");

  Serial.print("Writing reg <- [IMM] LOD instructions .");
  for (uint8_t dreg = Ra; dreg <= PC; dreg++) {

    write_instruction(OPCODE(LOD, dreg, IMM), MICROCODE3(_MAW | _PCE | PCC, _ME | PGM | _MAW, _ME | _W(dreg)), rom_no); 
  }  
  Serial.println(". done.");

  Serial.print("Writing reg <- [Rc] LDP instructions .");
  for(uint8_t dreg = Ra; dreg <= PC; dreg++) {

    write_instruction(OPCODE(LOD,IMM,dreg), MICROCODE2(_MAW | _RcE, _ME | PGM | _W(dreg)), rom_no);
  }
  Serial.println(". done.");

  Serial.println("48 LOD instructions written.");
  Serial.print("Writing HLT to currently unused opcodes (10 total): LOD IMM, [SPi|IMM] (2)  |  LOD SPi, [<any_R>|SPi|IMM] (8) ");  
  for (uint8_t sreg = Ra; sreg <= IMM; sreg++) {

    write_instruction(OPCODE(LOD, SPi, sreg), MICROCODE1(_HLT), rom_no); 
  }
  Serial.print(".");
  for (uint8_t sreg = SPi; sreg <= IMM; sreg++) {

    write_instruction(OPCODE(LOD, IMM, sreg), MICROCODE1(_HLT), rom_no); 
  }
  Serial.print(".");
  Serial.println(" done.");

}
#endif

#ifdef INS_STO
void write_STOs(uint8_t rom_no) {

  Serial.print("Writing [reg] <- reg STO instructions ");
  for (uint8_t sreg = Ra; sreg <= PC; sreg++) {

    Serial.print(".");
    for (uint8_t dreg = Ra; dreg <= PC; dreg++) {

      write_instruction(OPCODE(STO, dreg, sreg), MICROCODE2(_MAW | _E(dreg), _MW | _E(sreg)), rom_no);
    }
  }
  Serial.println(" done.");

  Serial.print("Writing [SPi] <- reg PUSH instructions .");
  for (uint8_t sreg = Ra; sreg <= SP; sreg++) {

    write_instruction(OPCODE(STO, SPi, sreg), MICROCODE3(_SPE | _ALW | ALS(A_MINUS_B), _SPW | _ALE | _MAW, _E(sreg) | _MW), rom_no); 
  }
  Serial.println(". done.");

  Serial.print("Writing CALL RC instruction .");
  write_instruction(OPCODE(STO, SPi, PC), MICROCODE4(_SPE | _ALW | ALS(A_MINUS_B), _SPW | _ALE | _MAW, _PCE | _MW, _PCW | _RcE), rom_no);
  Serial.println(". done.");

  Serial.print("Writing CALL #IMM instruction .");
  write_instruction(OPCODE(STO, SPi, IMM), MICROCODE5(_SPE | _ALW | ALS(A_MINUS_B), _SPW | _MAW | _ALE | PCC, _PCE | _MW | _ALW | ALS(A_MINUS_B), _ALE | _MAW, _ME | PGM | _PCW), rom_no);
  Serial.println(". done.");
  
  Serial.print("Writing [IMM] <- reg STO instructions .");
  for (uint8_t sreg = Ra; sreg <= PC; sreg++) {

    write_instruction(OPCODE(STO, IMM, sreg), MICROCODE3(_MAW | _PCE | PCC, _ME | PGM | _MAW, _MW | _E(sreg)), rom_no);
  }
  Serial.println(". done.");

  Serial.println("48 STO instructions written.");
  Serial.print("Writing HLT to currently unused opcodes (15 total): STO [<any_R>|IMM] <- IMM (7)  |  STO [<any_R>|SPi|IMM] <- SPi (8) ");

  for (uint8_t sreg = SPi; sreg <= IMM; sreg++) {
    for (uint8_t dreg = Ra; dreg <= IMM; dreg++) {

      if ((dreg != SPi) || (sreg != IMM)) {
          write_instruction(OPCODE(STO, dreg, sreg), MICROCODE1(_HLT), rom_no); 
      }
    }
    Serial.print(".");
  }  
  Serial.println(" done.");
}
#endif

#ifdef INS_ALU
void write_ALU_instructions(uint8_t rom_no) {

  Serial.print("Writing INC reg <- reg + 1 and DEC reg <- reg - 1 instructions .");
  for (uint8_t reg = Ra; reg <= Rd; reg++) {

    write_instruction(ALU_OPCODE(false, INC_A, reg), MICROCODE2(_E(reg) | ALS(A_PLUS_B) | ALC | _ALW, _W(reg) | _ALE), rom_no);
  }
  Serial.print(".");
  
  for (uint8_t reg = Ra; reg <= Rd; reg++) {

    write_instruction(ALU_OPCODE(true, INC_A, reg), MICROCODE2(_E(reg) | ALS(A_MINUS_B) | _ALW, _W(reg) | _ALE), rom_no);
  }
  Serial.println(". done.");

  Serial.print("Writing ADD|ADC reg <- reg + Rb instructions .");
  for (uint8_t reg = Ra; reg <= Rd; reg++) {

    write_instruction(ALU_OPCODE(false, A_PLUS_B, reg), MICROCODE2(_E(reg) | _ALB | ALS(A_PLUS_B) | _ALW, _W(reg) | _ALE), rom_no);
    write_carrycond_instruction(ALU_OPCODE(true, A_PLUS_B, reg), false, MICROCODE2(_E(reg) | _ALB | ALS(A_PLUS_B) | _ALW, _W(reg) | _ALE), rom_no);
    write_carrycond_instruction(ALU_OPCODE(true, A_PLUS_B, reg), true, MICROCODE2(_E(reg) | _ALB | ALS(A_PLUS_B) | _ALW | ALC, _W(reg) | _ALE), rom_no);    
  }
  Serial.println(". done.");

  Serial.print("Writing SUB|SBC reg <- reg - Rb instructions .");
  for (uint8_t reg = Ra; reg <= Rd; reg++) {

    write_instruction(ALU_OPCODE(false, A_MINUS_B, reg), MICROCODE2(_E(reg) | _ALB | ALC | ALS(A_MINUS_B) | _ALW, _W(reg) | _ALE), rom_no);
    write_carrycond_instruction(ALU_OPCODE(true, A_MINUS_B, reg), false, MICROCODE2(_E(reg) | _ALB | ALS(A_MINUS_B) | _ALW, _W(reg) | _ALE), rom_no);
    write_carrycond_instruction(ALU_OPCODE(true, A_MINUS_B, reg), true, MICROCODE2(_E(reg) | _ALB | ALS(A_MINUS_B) | _ALW | ALC, _W(reg) | _ALE), rom_no);    
  }
  Serial.println(". done.");

  Serial.print("Writing SUB|SBC reg <- Rb - reg instructions .");
  for (uint8_t reg = Ra; reg <= Rd; reg++) {

    write_instruction(ALU_OPCODE(false, B_MINUS_A, reg), MICROCODE2(_E(reg) | _ALB | ALC | ALS(B_MINUS_A) | _ALW, _W(reg) | _ALE), rom_no);
    write_carrycond_instruction(ALU_OPCODE(true, B_MINUS_A, reg), false, MICROCODE2(_E(reg) | _ALB | ALS(B_MINUS_A) | _ALW, _W(reg) | _ALE), rom_no);
    write_carrycond_instruction(ALU_OPCODE(true, B_MINUS_A, reg), true, MICROCODE2(_E(reg) | _ALB | ALS(B_MINUS_A) | _ALW | ALC, _W(reg) | _ALE), rom_no);    
  }
  Serial.println(". done.");

  Serial.print("Writing AND reg <- reg & Rb instructions .");
  for (uint8_t reg = Ra; reg <= Rd; reg++) {

    write_instruction(ALU_OPCODE(false, A_AND_B, reg), MICROCODE2(_E(reg) | _ALB | ALS(A_AND_B) | _ALW, _W(reg) | _ALE), rom_no);
  }
  Serial.println(". done.");
  
  Serial.print("Writing OR reg <- reg | Rb instructions .");
  for (uint8_t reg = Ra; reg <= Rd; reg++) {

    write_instruction(ALU_OPCODE(false, A_OR_B, reg), MICROCODE2(_E(reg) | _ALB | ALS(A_OR_B) | _ALW, _W(reg) | _ALE), rom_no);
  }
  Serial.println(". done.");
  
  Serial.print("Writing XOR reg <- reg ^ Rb instructions .");
  for (uint8_t reg = Ra; reg <= Rd; reg++) {

    write_instruction(ALU_OPCODE(false, A_XOR_B, reg), MICROCODE2(_E(reg) | _ALB | ALS(A_XOR_B) | _ALW, _W(reg) | _ALE), rom_no);
  }
  Serial.println(". done.");

  Serial.print("Writing NOT reg <- !reg instructions .");
  for (uint8_t reg = Ra; reg <= Rd; reg++) {

    write_instruction(ALU_OPCODE(false, NOT_A, reg), MICROCODE2(_E(reg) | ALS(B_MINUS_A) | _ALW, _W(reg) | _ALE), rom_no);
  }
  Serial.println(". done.");

  Serial.print("Writing CMP Rb, reg instructions .");
  for (uint8_t reg = Ra; reg <= Rd; reg++) {

    write_instruction(ALU_OPCODE(true, A_OR_B, reg), MICROCODE1(_E(reg) | ALC | _ALB | ALS(B_MINUS_A) | _ALW), rom_no);
  }
  Serial.println(". done.");

  Serial.print("Writing CMP reg, Rb instructions .");
  for (uint8_t reg = Ra; reg <= Rd; reg++) {

    write_instruction(ALU_OPCODE(true, A_AND_B, reg), MICROCODE1(_E(reg) | ALC | _ALB | ALS(A_MINUS_B) | _ALW), rom_no);
  }
  Serial.println(". done.");

  Serial.print("Writing TST reg instructions .");
  for (uint8_t reg = Ra; reg <= Rd; reg++) {

    write_instruction(ALU_OPCODE(true, NOT_A, reg), MICROCODE1(_E(reg) | ALS(A_PLUS_B) | _ALW), rom_no);
  }
  Serial.println(". done.");

  Serial.println("Written 60 ALU instructions.");
  Serial.print("Writing HLT to currently unused instructions (4 total): XORC .");
  for (uint8_t reg = Ra; reg <= Rd; reg++) {
    write_instruction(ALU_OPCODE(true, A_XOR_B, reg), MICROCODE1(_HLT), rom_no);
  }
  Serial.println(". done.");
}
#endif

#ifdef CHECK_COVERAGE
char buf[80];
#endif

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  Serial.print("About to program microcode EEPROM no. ");
  Serial.print(ROM_NO);
  Serial.print(". Press 'y' to continue ... ");
  while (Serial.available() <= 0);
  char answer = Serial.read();
  Serial.println(answer);

  if (answer == 'y') {

    #ifdef CHECK_COVERAGE
    for (uint16_t c=0; c <1024; c++) {

        write_mask[c] = 0;
    }
    #endif
    
    MyEEPROM.init();
    
    #ifdef INS_MOV
    write_MOVs(ROM_NO);
    #endif
  
    #ifdef INS_LOD
    write_LODs(ROM_NO);
    #endif
  
    #ifdef INS_STO
    write_STOs(ROM_NO);
    #endif
    
    #ifdef INS_ALU
    write_ALU_instructions(ROM_NO);
    #endif
  
    #ifdef DUMP
    MyEEPROM.dump_eeprom(0, 0x1FFF);
    #endif

    #ifdef CHECK_COVERAGE
    Serial.println("Output mask");
    for (int c=0; c < 32; c+=16) {

      sprintf(buf, "%02x %02x %02x %02x %02x %02x %02x %02x  %02x %02x %02x %02x %02x %02x %02x %02x",
      write_mask[c], write_mask[c+1], write_mask[c+2], write_mask[c+3], write_mask[c+4], write_mask[c+5], write_mask[c+6], write_mask[c+7],
      write_mask[c+8], write_mask[c+9], write_mask[c+10], write_mask[c+11], write_mask[c+12], write_mask[c+13], write_mask[c+14], write_mask[c+15]);
      Serial.println(buf);
    }
    #endif
  }  
  Serial.println("Have a nice day.");
}

// the loop function runs over and over again forever
void loop() {
}
