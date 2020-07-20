#include <nan.h>
#include "ACore/ACore.h"

class ACoreWrapper : public Nan::ObjectWrap
{
    public:
        static NAN_METHOD(AddDevice);
        static NAN_METHOD(RemoveDevice);
        static NAN_GETTER(Devices);

        static NAN_GETTER(Initialized);
        static NAN_METHOD(Initialize);
        static NAN_METHOD(Deinitialize);

        static NAN_METHOD(Process);
        static NAN_METHOD(Execute);
};
