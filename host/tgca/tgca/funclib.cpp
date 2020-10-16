#include "funclib.h"

QByteArray cmdHead(int cmd, int dsz)
{
    QByteArray head;
    head.append((char*)&cmd, 4);
    head.append((char*)&dsz, 4);
    return head;
}

FormatData::FormatData(ExchangeFormat f, int size) : size(size)
{
    protocolFrame = new char[this->size];
    exf = f;
    switch (exf) {
    case ExchangeFormat::write_f1:
        offset = 8;
        break;
    case ExchangeFormat::write_f2:
        offset = 12;
        break;
    case ExchangeFormat::read_f1:
        offset = 8;
        break;
    case ExchangeFormat::read_f2:
        offset = 16;
        break;
    case ExchangeFormat::write_echo:
        offset = 8;
        break;
    case ExchangeFormat::cpy_on_hard:
        offset = 20;
        break;
    }
    payload = protocolFrame + offset;
    curPayloadPoint = offset;
    frameSize = (unsigned int*)protocolFrame + 1;
    *frameSize = offset;
}

FormatData::~FormatData()
{
    delete[] protocolFrame;
}

void FormatData::append(char *str, int size)
{

}

void FormatData::clear()
{
    *frameSize = offset;
    curPayloadPoint = offset;
}

const char *FormatData::getAllData()
{
    return protocolFrame;
}
