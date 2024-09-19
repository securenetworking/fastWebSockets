#include <iostream>
#include <cassert>

#include "../src/QueryParser.h"

int main() {

    {
        std::string buf = "?test1=&test2=someValue";
        assert(fWS::getDecodedQueryValue("test2", (char *) buf.data()) == "someValue");
    }

    {
        std::string buf = "?test1=&test2=someValue";
        assert(fWS::getDecodedQueryValue("test1", (char *) buf.data()) == "");
        assert(fWS::getDecodedQueryValue("test2", (char *) buf.data()) == "someValue");
    }

    {
        std::string buf = "?Kest1=&test2=someValue";
        assert(fWS::getDecodedQueryValue("test2", (char *) buf.data()) == "someValue");
    }

    {
        std::string buf = "?Test1=&Kest2=some";
        assert(fWS::getDecodedQueryValue("Test1", (char *) buf.data()) == "");
        assert(fWS::getDecodedQueryValue("Kest2", (char *) buf.data()) == "some");
    }

    {
        std::string buf = "?Test1=&Kest2=some";
        assert(fWS::getDecodedQueryValue("Test1", (char *) buf.data()).data() != nullptr);
        assert(fWS::getDecodedQueryValue("sdfsdf", (char *) buf.data()).data() == nullptr);
    }

    {
        std::string buf = "?Kest1=&test2=some%20Value";
        assert(fWS::getDecodedQueryValue("test2", (char *) buf.data()) == "some Value");
    }

    return 0;
}