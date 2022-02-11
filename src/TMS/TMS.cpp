#include <TMS/TMS.hpp>

namespace TMS {

CO_OBJ_T* TMS::getObjectDictionary() {
    return &objectDictionary[0];
}
uint16_t TMS::getObjectDictionarySize() {
    return OBJECT_DICTIONARY_SIZE;
}
}
