#include "peMatrixBase.h"

#include "configuration.h"
#include "detect/ScanI2C.h"

#ifdef INPUTBROKER_PEMATRIX_TYPE

I2CKeyPad keyPad(PCF8574A_ADDR, &Wire);  

PeMatrixBase::PeMatrixBase(const char *name) : concurrency::OSThread(name)
{
    this->_originName = name;
}

void PeMatrixBase::erase()
{
    InputEvent e;
    e.inputEvent = INPUT_BROKER_BACK; 
    e.kbchar = 0x08;
    e.source = this->_originName;
    this->notifyObservers(&e);
}

void PeMatrixBase::changeshift()
{
    InputEvent e;
    e.inputEvent = INPUT_BROKER_NONE;
    e.source = this->_originName;
        if (fnPe == 0){ // STERING ====
            keyPad.loadKeyMap(keymap1);
            e.kbchar = INPUT_BROKER_MSG_SYS_ON_SYMBOL_OFF;
        } else if (fnPe == 1){ //abc 
            keyPad.loadKeyMap(keymap2);
            e.kbchar = INPUT_BROKER_MSG_SYS_OFF_abc_SYMBOL_ON;
        } else if (fnPe == 2){ //ABC
            e.kbchar = INPUT_BROKER_MSG_SYS_OFF_ABC_SYMBOL_ON;
        } else if (fnPe == 3){  //123
            keyPad.loadKeyMap(keymap3);
            e.kbchar = INPUT_BROKER_MSG_SYS_OFF_123_SYMBOL_ON;
        }
    this->notifyObservers(&e);
}

int32_t PeMatrixBase::runOnce()
{
    if (!INPUTBROKER_PEMATRIX_TYPE) {
        // Input device is not requested.
        return disable();
    }
    
    if (firstTime) {
        // This is the first time the OSThread library has called this function, so do port setup
        firstTime = 0;
        if (!keyPad.begin()) {
            LOG_ERROR("Failed to initialize I2C keypad");
            return disable();
        }
        keyPad.setKeyPadMode(I2C_KEYPAD_5x3);
        keyPad.loadKeyMap(keymap1);
    } 

    if (INPUTBROKER_PEMATRIX_TYPE == 1) {
        key = keyPad.getChar();
        // if (millis() - debugTime > 1000) { //========================================
            // LOG_DEBUG("PeMatrixBase pressed -.-.-.-.- Key 0x%x pressed\n", key);
            // debugTime = millis();
        // }
        // debounce
        if (millis() - lastPressTime > debounceTime) {
            quickPress = 0;
            quickPressSpecial = 0;
        }
        if (key != prevkey) {
            if (key != 0x00) {
                LOG_DEBUG("Key 0x%x pressed", key);
                // reset fnPe now that we have a keypress
                InputEvent e;
                e.inputEvent = INPUT_BROKER_NONE;
                e.source = this->_originName;
                switch (key) {
                case 0x1b: // ESC
                    if(fnPe != 0){
                        fnPe = 0;
                        changeshift();
                        // shift = 1;
                    } else {
                        e.inputEvent = INPUT_BROKER_CANCEL;
                    }
                    break;
                case 0x08: // Back
                    e.inputEvent = INPUT_BROKER_BACK;
                    e.kbchar = key;
                    break;
                case 0xb5: // Up
                    e.inputEvent = INPUT_BROKER_UP;
                    e.kbchar = key;
                    break;
                case 0xb6: // Down
                    e.inputEvent = INPUT_BROKER_DOWN;
                    e.kbchar = key;
                    break;
                case 0xb4: // Left
                    e.inputEvent = INPUT_BROKER_LEFT;
                    e.kbchar = key;
                    break;
                case 0xb7: // Right
                    e.inputEvent = INPUT_BROKER_RIGHT;
                    e.kbchar = key;
                    break;
                case 0x0d: // Enter
                    e.inputEvent = INPUT_BROKER_SELECT;
                    break;
                case 0xcc: // function 
                    fnPe += 1;
                    if (fnPe > 3) {
                        fnPe = 1;
                    }
                    changeshift();
                    break;
                case 0x00: // nopress
                    e.inputEvent = INPUT_BROKER_NONE;
                    break;
                case 0x20: // space
                    e.inputEvent = INPUT_BROKER_ANYKEY;
                    e.kbchar = key;
                    break;
                default: // all other keys
                    if (fnPe == 0){
                        e.inputEvent = INPUT_BROKER_ANYKEY;
                        e.kbchar = key;
                    } else if ((fnPe == 1) || (fnPe == 2)){
                        if (key == lastKeyPressed && millis() - lastPressTime < debounceTime) {
                            quickPress += 1;
                            if (key == 0x50 || key == 0x57) { //P and W
                                if (quickPress > 3) {
                                    quickPress = 0;
                                }
                            } else if (key == 0x2c) {
                                if (quickPress > 3) {
                                    quickPress = 0;
                                }
                                switch (quickPress){
                                case 0/* , */:
                                    quickPressSpecial = 0 - quickPress;/* code */
                                    break;
                                case 1/* . */:
                                    quickPressSpecial = 2 - quickPress;/* code */
                                    break;
                                case 2/* ? */:
                                    quickPressSpecial = 19 - quickPress;/* code */
                                    break;
                                case 3/* ASCII " ! " = 44 */:
                                    quickPressSpecial = -11 - quickPress; 
                                    break;
                                default:
                                    break;
                                }
                            } else {
                                if (quickPress > 2) {
                                    quickPress = 0; 
                                }
                            }
                        }
                        if ((fnPe == 2) || (key == 0x2c)){
                            quickPressshift = 0; 
                        } else if (fnPe == 1){    
                            quickPressshift = 32;
                        }
                        if (key != lastKeyPressed) {
                            quickPress = 0;
                            quickPressSpecial = 0;
                        }
                        if (key == lastKeyPressed && millis() - lastPressTime < debounceTime) {
                            erase();
                        }
                        e.inputEvent = INPUT_BROKER_ANYKEY;
                        e.kbchar = key + quickPress + quickPressSpecial + quickPressshift;
                    } else if (fnPe == 3){
                        e.inputEvent = INPUT_BROKER_ANYKEY;
                        e.kbchar = key;
                    }
                    lastPressTime = millis();
                    lastKeyPressed = key;
                    break;
                }         
                if (e.inputEvent != INPUT_BROKER_NONE) {
                    this->notifyObservers(&e);
                }
            }
            prevkey = key;
        }
    }
    return 50; // Keyscan every 50msec to avoid key bounce
} 
#endif // INPUTBROKER_MATRIX_TYPE