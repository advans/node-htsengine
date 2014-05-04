#include <node.h>
#include "nan.h"
#include <stdlib.h>
#include "hts_engine_api.h"

#define WRITE_U32(buf, x) *(buf)     = (unsigned char)((x)&0xff);\
    *((buf)+1) = (unsigned char)(((x)>>8)&0xff);\
    *((buf)+2) = (unsigned char)(((x)>>16)&0xff);\
    *((buf)+3) = (unsigned char)(((x)>>24)&0xff);

#define WRITE_U16(buf, x) *(buf)     = (unsigned char)((x)&0xff);\
    *((buf)+1) = (unsigned char)(((x)>>8)&0xff);


v8::Local<v8::Object> HTSEngineNodeWave::create(HTS_Engine *engine)
{
    int wavFileSize = engine->gss.total_nsample * sizeof(short) + 44;
    char *wavData = (char *) malloc(wavFileSize);

    char data_01_04[] = { 'R', 'I', 'F', 'F' };
    //int data_05_08 = HTS_GStreamSet_get_total_nsamples(gss) * sizeof(short) + 36;
    int data_05_08 = engine->gss.total_nsample * sizeof(short) + 36;
    char data_09_12[] = { 'W', 'A', 'V', 'E' };
    char data_13_16[] = { 'f', 'm', 't', ' ' };
    int data_17_20 = 16;
    short data_21_22 = 1;        /* PCM */
    short data_23_24 = 1;        /* monoral */
    int data_25_28 = engine->condition.sampling_frequency;
    int data_29_32 = engine->condition.sampling_frequency * sizeof(short);
    short data_33_34 = sizeof(short);
    short data_35_36 = (short) (sizeof(short) * 8);
    char data_37_40[] = { 'd', 'a', 't', 'a' };
    //int data_41_44 = HTS_GStreamSet_get_total_nsamples(gss) * sizeof(short);
    int data_41_44 = engine->gss.total_nsample * sizeof(short);

    /* write header */
    //HTS_fwrite_little_endian(data_01_04, sizeof(char), 4, fp);
    //WRITE_U16(wavData, data_01_04);
    memcpy   (wavData     , data_01_04, 4);
    // HTS_fwrite_little_endian(&data_05_08, sizeof(int), 1, fp);
    WRITE_U32(wavData +  4, data_05_08);
    // HTS_fwrite_little_endian(data_09_12, sizeof(char), 4, fp);
    memcpy   (wavData +  8, data_09_12, 4);
    // HTS_fwrite_little_endian(data_13_16, sizeof(char), 4, fp);
    memcpy   (wavData + 12, data_13_16, 4);
    // HTS_fwrite_little_endian(&data_17_20, sizeof(int), 1, fp);
    WRITE_U32(wavData + 16, data_17_20);
    // HTS_fwrite_little_endian(&data_21_22, sizeof(short), 1, fp);
    WRITE_U16(wavData + 20, data_21_22);
    // HTS_fwrite_little_endian(&data_23_24, sizeof(short), 1, fp);
    WRITE_U16(wavData + 22, data_23_24);
    // HTS_fwrite_little_endian(&data_25_28, sizeof(int), 1, fp);
    WRITE_U32(wavData + 24, data_25_28);
    // HTS_fwrite_little_endian(&data_29_32, sizeof(int), 1, fp);
    WRITE_U32(wavData + 28, data_29_32);
    // HTS_fwrite_little_endian(&data_33_34, sizeof(short), 1, fp);
    WRITE_U16(wavData + 32, data_33_34);
    // HTS_fwrite_little_endian(&data_35_36, sizeof(short), 1, fp);
    WRITE_U16(wavData + 34, data_35_36);
    // HTS_fwrite_little_endian(data_37_40, sizeof(char), 4, fp);
    memcpy   (wavData + 36, data_37_40, 4);
    // HTS_fwrite_little_endian(&data_41_44, sizeof(int), 1, fp);
    WRITE_U32(wavData + 40, data_41_44);
    /* write data */
    short *buf = (short *)malloc(engine->gss.total_nsample * sizeof(short));
    //for (i = 0; i < HTS_GStreamSet_get_total_nsamples(gss); i++) {
    for (size_t i = 0; i < engine->gss.total_nsample; i++)
    {
        buf[i] = engine->gss.gspeech[i];
    }
    memcpy(wavData + 44, buf, engine->gss.total_nsample * sizeof(short));
    return NanNewBufferHandle(wavData, wavFileSize);
};