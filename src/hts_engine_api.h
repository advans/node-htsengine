#include <node.h>
#include <vector>
#include <map>
#include <iostream>
#include "nan.h"

#include "HTS_engine.h"


class HTSEngineNode : public node::ObjectWrap
{
public:
    HTSEngineNode();
    ~HTSEngineNode();
    void initEngine(HTS_Engine *engine);
    bool loadHtsvoice(HTS_Engine *engine);
    void loadTest();
    /**
    * Static Methods
    */
    static void Init ();
    static NAN_METHOD(NewInstance);
    /**
     * Prototype Methods
     */
    static NAN_METHOD(synthesizeFormFileSync);
    static NAN_METHOD(synthesizeFormFile);
    static NAN_METHOD(synthesizeFormStringSync);
    static NAN_METHOD(synthesizeFormString);
    /**
     * Prototype Values
     */
    // htsvoice
    std::vector<std::string> htsvoices;
    static NAN_GETTER(GetHtsvoice);
    static NAN_SETTER(SetHtsvoice);
    // phonemeAlignment
    bool phonemeAlignment;
    static NAN_GETTER(GetPhonemeAlignment);
    static NAN_SETTER(SetPhonemeAlignment);
    // samplingFrequency
    int samplingFrequency;
    static NAN_GETTER(GetSamplingFrequency);
    static NAN_SETTER(SetSamplingFrequency);
    // framePeriod
    int framePeriod;
    static NAN_GETTER(GetFramePeriod);
    static NAN_SETTER(SetFramePeriod);
    // allPassConstant
    double allPassConstant;
    static NAN_GETTER(GetAllPassConstant);
    static NAN_SETTER(SetAllPassConstant);
    // postFilteringCoefficient
    double postFilteringCoefficient;
    static NAN_GETTER(GetPostFilteringCoefficient);
    static NAN_SETTER(SetPostFilteringCoefficient);
    // speechSpeedRate
    double speechSpeedRate;
    static NAN_GETTER(GetSpeechSpeedRate);
    static NAN_SETTER(SetSpeechSpeedRate);
    // additionalHalfTone
    double additionalHalfTone;
    static NAN_GETTER(GetAdditionalHalfTone);
    static NAN_SETTER(SetAdditionalHalfTone);
    // unvoicedThreshold
    double unvoicedThreshold;
    static NAN_GETTER(GetUnvoicedThreshold);
    static NAN_SETTER(SetUnvoicedThreshold);
    // interpolation
    std::map<std::string, double> interpolation;
    static NAN_GETTER(GetInterpolation);
    static NAN_SETTER(SetInterpolation);
private:
    static NAN_METHOD(New);

};

class HTSEngineNodeWave
{
public:
    v8::Local<v8::Object> create(HTS_Engine *engine);
private:
    HTS_Engine engine;
};

class HTSEngineNodeFileAsyncWorker : public NanAsyncWorker
{
public:
    HTSEngineNodeFileAsyncWorker(NanCallback *callback, HTS_Engine engine, char *fileName);
    ~HTSEngineNodeFileAsyncWorker();
    virtual void Execute();
    void HandleOKCallback ();
private:
    HTS_Engine engine;
    char *fileName;
};


class HTSEngineNodeStringAsyncWorker : public NanAsyncWorker
{
public:
    HTSEngineNodeStringAsyncWorker(NanCallback *callback, HTS_Engine engine, v8::Handle<v8::Array> labelArray);
    ~HTSEngineNodeStringAsyncWorker();
    virtual void Execute();
    void HandleOKCallback ();
private:
    HTS_Engine engine;
    size_t num_lines;
    char **lines;
    v8::Handle<v8::Array> labelArray;
};