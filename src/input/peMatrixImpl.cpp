#include "peMatrixImpl.h"
#include "InputBroker.h"

#ifdef INPUTBROKER_PEMATRIX_TYPE

PeMatrixImpl *peMatrixImpl;

PeMatrixImpl::PeMatrixImpl() : PeMatrixBase("matrixPE") {}

void PeMatrixImpl::init()
{
    // if (kb_model != 0x12) {
    if (!INPUTBROKER_PEMATRIX_TYPE) {
        LOG_ERROR("I2C keypad ........is config PeMatrixImpl");
        disable();
        return;
    }

    inputBroker->registerSource(this);
}

#endif // INPUTBROKER_MATRIX_TYPE