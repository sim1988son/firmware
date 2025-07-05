#pragma once

#include "InputBroker.h"
#include "concurrency/OSThread.h"
#include "I2CKeyPad.h"

class PeMatrixBase : public Observable<const InputEvent *>, public concurrency::OSThread
{
  public:
    explicit PeMatrixBase(const char *name);

  protected:
    virtual int32_t runOnce() override;
    void erase();
    void changeshift();

  private:
    const char *_originName;
    bool firstTime = 1;
    // char keymap[19] = "1234567890ABCDENF";  // N = NoKey, F = Fail

  char keymap1[18] = {0x1b, 0x12, 0xb5, 0x11, 0x8b,  // ESC     BL-     UP      BL+     BACK
                      0x09, 0xb4, 0x0d, 0xb7, 0X00,  // TAB     LEFT    ENTER   RIGHT
                      0xAA, 0x9e, 0xb6, 0xac, 0xcc,  // BT      GPS     DOWN    NOTYFI  FN
                      0x00, 0x00};
  char keymap2[18] = {0x1b, ',' , 'A' , 'D' , 0x08,  // ESC     ,.?!    ABC     DEF     BACK
                      0x09, 'G' , 'J' , 'M' , 0x20,  // TAB     GHI     JKL     MNO     SPAC
                      0x8F, 'P' , 'T' , 'W' , 0xcc,  // SHIFT   PQRS    TUV     WXYZ    FN
                      0x00, 0x00};
  char keymap3[18] = {0x1b, '1' , '2' , '3' , 0x08,  // ESC     1       2       3       BACK
                      0x09, '4' , '5' , '6' , '0',   // TAB     4       5       6       0
                      0x8F, '7' , '8' , '9' , 0xcc,  //         7       8       9       FN
                      0x00, 0x00};

    char key = 0;
    char prevkey = 0;
    int lastKeyPressed = 13;
    int shift = 1;
    int fnPe = 0;
    int firstshift = 0;
    int quickPress = 0;
    int quickPressSpecial = 0;
    int quickPressshift = 0;
    unsigned long lastPressTime = 0;
    unsigned long debugTime = 0;
    unsigned long debounceTime = 700;
    int changefnPe = 0;
    // int changeshift = 0;
    
  
}; 