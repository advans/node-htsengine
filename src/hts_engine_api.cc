#include <node.h>
#include "nan.h"
#include <stdlib.h>
#include "hts_engine_api.h"

using namespace v8;

//
static Persistent<FunctionTemplate> enginePersistent;

/**
 * [HTSEngineNode::HTSEngineNode constructor]
 */
HTSEngineNode::HTSEngineNode ()
{
#ifdef HTS_ENGINE_DEBUG
    fprintf(stderr, "HTSEngineNode::HTSEngineNode\n");
#endif
    //num_voices = 0;
    phonemeAlignment = false;
    unvoicedThreshold = 0.5;
    additionalHalfTone = 0;
    speechSpeedRate = 1;
    postFilteringCoefficient = 0;
    allPassConstant = 0.55;
    framePeriod = 240;
    samplingFrequency = 48000;
};
void HTSEngineNode::initEngine(HTS_Engine *engine)
{

#ifdef HTS_ENGINE_DEBUG
    fprintf(stderr, "HTSEngineNode::initEngine\n");
#endif
    HTS_Engine_initialize(engine);

    loadHtsvoice(engine);

    HTS_Engine_set_phoneme_alignment_flag(engine, phonemeAlignment);

    HTS_Engine_set_sampling_frequency(engine, (size_t) samplingFrequency);

    HTS_Engine_set_fperiod(engine, (size_t) framePeriod);

    HTS_Engine_set_alpha(engine, allPassConstant);

    HTS_Engine_set_beta(engine, postFilteringCoefficient);

    HTS_Engine_set_speed(engine, speechSpeedRate);

    HTS_Engine_add_half_tone(engine, additionalHalfTone);

    HTS_Engine_set_msd_threshold(engine, 1, unvoicedThreshold);
}
/**
 * [HTSEngineNode::loadHtsvoice description]
 * @param  engine [description]
 * @return        [description]
 */
bool HTSEngineNode::loadHtsvoice(HTS_Engine *engine)
{
#ifdef HTS_ENGINE_DEBUG
    fprintf(stderr, "HTSEngineNode::loadHtsvoice\n");
#endif
    /* HTS voices */
    size_t num_voices;
    char **fn_voices;
    /* get HTS voice file names */
    num_voices = htsvoices.size();
    fn_voices = (char **) malloc(num_voices * sizeof(char *));
    for (std::vector<std::string>::iterator it = htsvoices.begin(); it != htsvoices.end(); ++it)
    {
        size_t index = std::distance(htsvoices.begin(), it);
        char *fn_voice = const_cast<char *>(it->c_str());
        fn_voices[index] = fn_voice;
    }
    return HTS_Engine_load(engine, fn_voices, num_voices);
}
/**
 * [HTSEngineNode::loadTest description]
 */
void HTSEngineNode::loadTest()
{
    /* hts_engine API */
    HTS_Engine engine;

    /* HTS voices */
    size_t num_voices;
    char **fn_voices;

    HTS_Engine_initialize(&engine);

    /* get HTS voice file names */
    num_voices = htsvoices.size();
    fn_voices = (char **) malloc(num_voices * sizeof(char *));
    for (std::vector<std::string>::iterator it = htsvoices.begin(); it != htsvoices.end(); ++it)
    {
        size_t index = std::distance(htsvoices.begin(), it);
        char *fn_voice = const_cast<char *>(it->c_str());
        fn_voices[index] = fn_voice;
    }
    /* load HTS voices */
    if (HTS_Engine_load(&engine, fn_voices, num_voices) != TRUE)
    {
        htsvoices.clear();
        interpolation.clear();
    }
    else
    {
        if (interpolation.empty())
        {
            for (std::vector<std::string>::iterator it = htsvoices.begin(); it != htsvoices.end(); ++it)
            {
                size_t index = std::distance(htsvoices.begin(), it);

                double interpolationWeight = HTS_Engine_get_duration_interpolation_weight(&engine, index);
                interpolation[it->c_str()] = interpolationWeight;
                char *fn_voice = const_cast<char *>(it->c_str());
                fn_voices[index] = fn_voice;
            }
        }
    }
    /* reset */
    HTS_Engine_refresh(&engine);

    /* free memory */
    HTS_Engine_clear(&engine);
    free(fn_voices);

}
/**
 * [HTSEngineNode::~HTSEngineNode  destructor]
 */
HTSEngineNode::~HTSEngineNode ()
{
#ifdef HTS_ENGINE_DEBUG
    fprintf(stderr, "HTSEngineNode::~HTSEngineNode\n");
#endif
};

/**
 * [HTSEngineNode::Init  description]
 */
void HTSEngineNode::Init ()
{
    //
#ifdef HTS_ENGINE_DEBUG
    fprintf(stderr, "HTSEngineNode::Init!\n");
#endif
    Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
    NanAssignPersistent(FunctionTemplate, enginePersistent, tpl);
    tpl->SetClassName(NanSymbol("HTSEngine"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    //
    // Prototype Method
    NODE_SET_PROTOTYPE_METHOD(tpl, "synthesizeFormFileSync"  , synthesizeFormFileSync);
    NODE_SET_PROTOTYPE_METHOD(tpl, "synthesizeFormFile",       synthesizeFormFile);
    NODE_SET_PROTOTYPE_METHOD(tpl, "synthesizeFormStringSync", synthesizeFormStringSync);
    NODE_SET_PROTOTYPE_METHOD(tpl, "synthesizeFormString",     synthesizeFormString);
    // Prototypes values
    Local<ObjectTemplate> proto = tpl->PrototypeTemplate();
#ifdef HTS_ENGINE_DEBUG
    proto = tpl->InstanceTemplate();
#endif
    //htsvoice
    proto->SetAccessor(NanSymbol("htsvoice"), GetHtsvoice, SetHtsvoice);
    proto->SetAccessor(NanSymbol("phonemeAlignment"), GetPhonemeAlignment, SetPhonemeAlignment);
    proto->SetAccessor(NanSymbol("samplingFrequency"), GetSamplingFrequency, SetSamplingFrequency);
    proto->SetAccessor(NanSymbol("framePeriod"), GetFramePeriod, SetFramePeriod);
    proto->SetAccessor(NanSymbol("allPassConstant"), GetAllPassConstant, SetAllPassConstant);
    proto->SetAccessor(NanSymbol("postFilteringCoefficient"), GetPostFilteringCoefficient, SetPostFilteringCoefficient);
    proto->SetAccessor(NanSymbol("speechSpeedRate"), GetSpeechSpeedRate, SetSpeechSpeedRate);
    proto->SetAccessor(NanSymbol("additionalHalfTone"), GetAdditionalHalfTone, SetAdditionalHalfTone);
    proto->SetAccessor(NanSymbol("unvoicedThreshold"), GetUnvoicedThreshold, SetUnvoicedThreshold);
    proto->SetAccessor(NanSymbol("interpolation"), GetInterpolation, SetInterpolation);
};
/**
 * [New description]
 */
NAN_METHOD(HTSEngineNode::New)
{
    NanScope();
#ifdef HTS_ENGINE_DEBUG
    fprintf(stderr, "HTSEngineNode::New!\n");
#endif
    HTSEngineNode *nodeInstance = new HTSEngineNode();
    //nodeInstance->m_Count = 0;
    nodeInstance->Wrap(args.This());
    //args.This()->SetInternalField(0, v8::External::New(nodeInstance));
    if (args[0]->IsObject())
    {
        Handle<Object> configObject = Handle<Object>::Cast(args[0]);
        Local<Array> config = configObject->GetPropertyNames();
        for (uint32_t i = 0; i < config->Length(); i++)
        {
            Handle<String> key = config->Get(Integer::New(i))->ToString();
            Handle<Value> value = configObject->Get(key);
            if (args.This()->Has(key))
            {
                args.This()->Set(key, value);
            }

        }
    }
    // HTS_COPYRIGHT
    args.This()->Set(NanSymbol("HTS_COPYRIGHT"), NanSymbol(HTS_COPYRIGHT));

    NanReturnValue(args.This());
};
/**
 * [NewInstance description]
 * @param args [description]
 */
NAN_METHOD(HTSEngineNode::NewInstance)
{
    NanScope();
#ifdef HTS_ENGINE_DEBUG
    fprintf(stderr, "HTSEngineNode::NewInstance!\n");
#endif
    Local<Object> instance;
    Local<FunctionTemplate> constructorHandle = NanPersistentToLocal(enginePersistent);
    Handle<Value> argv[] = { args[0] };
    instance = constructorHandle->GetFunction()->NewInstance(1, argv);
    NanReturnValue(instance);
}
/**
 * Prototype Methods
 */
NAN_METHOD(HTSEngineNode::synthesizeFormFileSync)
{
    NanScope();
#ifdef HTS_ENGINE_DEBUG
    fprintf(stderr, "HTSEngineNode::synthesizeFormFileSync\n");
#endif
    HTSEngineNode *context = ObjectWrap::Unwrap<HTSEngineNode>(args.This());
    if (context->htsvoices.empty())
    {
        NanThrowError("HTS htsvoices cannot be loaded.");
    }
    // has File
    if (args[0]->IsString())
    {
        size_t count;
        char *fileName = NanCString(args[0]->ToString(), &count);
        HTS_Engine engine;
        context->initEngine(&engine);
#ifdef HTS_ENGINE_DEBUG
        fprintf(stderr, "synthesizeFormFileSync %s\n", fileName);
#endif
        /* synthesize */
        if (HTS_Engine_synthesize_from_fn(&engine, fileName) != TRUE)
        {
            HTS_Engine_clear(&engine);
            NanThrowError("HTS labelFile cannot be synthesized");
        }
        HTSEngineNodeWave *wav = new HTSEngineNodeWave();
        Local<Object> wavFileBuffer = wav->create(&engine);
        HTS_Engine_clear(&engine);
        free(fileName);
        free(wav);
        NanReturnValue(wavFileBuffer);

    };
    NanReturnUndefined();
}
NAN_METHOD(HTSEngineNode::synthesizeFormFile)
{
    NanScope();
#ifdef HTS_ENGINE_DEBUG
    fprintf(stderr, "HTSEngineNode::synthesizeFormFile\n");
#endif
    HTSEngineNode *context = ObjectWrap::Unwrap<HTSEngineNode>(args.This());
    if (context->htsvoices.empty())
    {
        NanThrowError("HTS htsvoices cannot be loaded.");
    }
    if (!args[0]->IsString() || !args[1]->IsFunction())
    {
        NanThrowError("use synthesizeFormFile(labelFile,callback)");
    }
    size_t count;
    char *fileName = NanCString(args[0]->ToString(), &count);
    HTS_Engine engine;
    context->initEngine(&engine);
#ifdef HTS_ENGINE_DEBUG
    fprintf(stderr, "synthesizeFormFile %s\n", fileName);
#endif

    NanCallback *successCallback = new NanCallback(args[1].As<Function>());
    NanAsyncQueueWorker(new HTSEngineNodeFileAsyncWorker(successCallback, engine, fileName));

    NanReturnUndefined();
};
/**
 *
 */
NAN_METHOD(HTSEngineNode::synthesizeFormStringSync)
{
    NanScope();
#ifdef HTS_ENGINE_DEBUG
    fprintf(stderr, "HTSEngineNode::synthesizeFormStringSync\n");
#endif
    HTSEngineNode *context = ObjectWrap::Unwrap<HTSEngineNode>(args.This());
    if (context->htsvoices.empty())
    {
        NanThrowError("HTS htsvoices cannot be loaded.");
    }
    if (!args[0]->IsArray())
    {
        NanThrowError("use synthesizeFormStringSync([labeltext])");
    }

    // args[0] to array
    Handle<Array> labelArray = Handle<Array>::Cast(args[0]);

    size_t num_lines;
    char **lines;
    /* get HTS voice file names */
    num_lines = labelArray->Length();
    lines = (char **) malloc(num_lines * sizeof(char *));

    for (uint32_t i = 0; i < labelArray->Length(); i++)
    {
        size_t count;
        Handle<String> labelString = labelArray->Get(Integer::New(i))->ToString();
        lines[i] = NanCString(labelString, &count);
    }
    // init HTS_Engine
    HTS_Engine engine;
    context->initEngine(&engine);
    /* synthesize */
    if (HTS_Engine_synthesize_from_strings(&engine, lines, num_lines) != TRUE)
    {
        HTS_Engine_clear(&engine);
        NanThrowError("HTS labelFile cannot be synthesized");
    }
    HTSEngineNodeWave *wav = new HTSEngineNodeWave();
    Local<Object> wavFileBuffer = wav->create(&engine);
    HTS_Engine_clear(&engine);
    free(lines);
    free(wav);
    NanReturnValue(wavFileBuffer);
};
NAN_METHOD(HTSEngineNode::synthesizeFormString)
{
    NanScope();
#ifdef HTS_ENGINE_DEBUG
    fprintf(stderr, "HTSEngineNode::synthesizeFormString\n");
#endif
    HTSEngineNode *context = ObjectWrap::Unwrap<HTSEngineNode>(args.This());
    if (context->htsvoices.empty())
    {
        NanThrowError("HTS htsvoices cannot be loaded.");
    }
    if (!args[0]->IsArray() || !args[1]->IsFunction())
    {
        NanThrowError("use synthesizeFormString([labeltext],callback)");
    }
    // args[0] to array
    Handle<Array> labelArray = Handle<Array>::Cast(args[0]);
    HTS_Engine engine;
    context->initEngine(&engine);
#ifdef HTS_ENGINE_DEBUG
    fprintf(stderr, "synthesizeFormString %d\n", labelArray->Length());
#endif

    NanCallback *successCallback = new NanCallback(args[1].As<Function>());
    NanAsyncQueueWorker(new HTSEngineNodeStringAsyncWorker(successCallback, engine, labelArray));

    NanReturnUndefined();
};
/**
 * Prototype Values
 */

//htsvoice getter setter
NAN_GETTER(HTSEngineNode::GetHtsvoice)
{
    NanScope();
#ifdef HTS_ENGINE_DEBUG
    fprintf(stderr, "HTSEngineNode::GetHtsvoice\n");
#endif
    HTSEngineNode *context = ObjectWrap::Unwrap<HTSEngineNode>(args.This());
    if ( context->htsvoices.size() == 0)
    {
        NanReturnUndefined();
    }
    else if (context->htsvoices.size() == 1)
    {
        NanReturnValue(String::New(context->htsvoices[0].c_str()));
    }
    else
    {
        // any htsvoice
        Local<Array> htsvoiceArray = Array::New(context->htsvoices.size());
        for (std::vector<std::string>::iterator it = context->htsvoices.begin(); it != context->htsvoices.end(); ++it)
        {
            size_t index = std::distance(context->htsvoices.begin(), it);
            htsvoiceArray->Set(Int32::New(index), String::New(it->c_str()));
        }
        NanReturnValue(htsvoiceArray);

    }
    NanReturnUndefined();

};
NAN_SETTER(HTSEngineNode::SetHtsvoice)
{
    NanScope();
#ifdef HTS_ENGINE_DEBUG
    fprintf(stderr, "HTSEngineNode::SetHtsvoice\n");
#endif
    HTSEngineNode *context = ObjectWrap::Unwrap<HTSEngineNode>(args.This());
    //  vector clean
    context->htsvoices.clear();
    context->interpolation.clear();
    if (value->IsString())
    {
        v8::String::Utf8Value htsvoice(value->ToString());
        context->htsvoices.push_back(std::string(*htsvoice));
    }
    else if (value->IsArray())
    {
        Handle<Array> array = Handle<Array>::Cast(value);
        for (uint32_t i = 0; i < array->Length(); i++)
        {

            Local<Value> item = array->Get(i);

            if (item->IsString())
            {
                // String
                String::Utf8Value htsvoice(item->ToString());
                context->htsvoices.push_back(std::string(*htsvoice));
            }
            else if (item->IsObject())
            {
                if (item->ToObject()->Has(NanSymbol("file")))
                {
                    String::Utf8Value htsvoice(item->ToObject()->Get(NanSymbol("file"))->ToString());
                    context->htsvoices.push_back(std::string(*htsvoice));
                    if (item->ToObject()->Has(NanSymbol("interpolation")))
                    {
                        Handle<Number> interpolationNumber = item->ToObject()->Get(NanSymbol("interpolation"))->ToNumber();
                        context->interpolation[std::string(*htsvoice)] = interpolationNumber->Value();
                    }
                }
            }
        }
    }
    // load test
    context->loadTest();
    if (context->htsvoices.size() == 0)
    {
        NanThrowError("HTS voices cannot be loaded.");
    }
};

// phonemeAlignment getter setter
NAN_GETTER(HTSEngineNode::GetPhonemeAlignment)
{
    NanScope();
#ifdef HTS_ENGINE_DEBUG
    fprintf(stderr, "HTSEngineNode::GetPhonemeAlignment\n");
#endif
    HTSEngineNode *context = ObjectWrap::Unwrap<HTSEngineNode>(args.This());
    NanReturnValue(Boolean::New(context->phonemeAlignment));
}
NAN_SETTER(HTSEngineNode::SetPhonemeAlignment)
{
    NanScope();
#ifdef HTS_ENGINE_DEBUG
    fprintf(stderr, "HTSEngineNode::SetPhonemeAlignment\n");
#endif
    if (value->IsBoolean())
    {
        HTSEngineNode *context = ObjectWrap::Unwrap<HTSEngineNode>(args.This());
        context->phonemeAlignment = value->ToBoolean()->Value();
    } else {
        NanThrowError("phonemeAlignment is Boolean.");
    }
}
// samplingFrequency getter setter
NAN_GETTER(HTSEngineNode::GetSamplingFrequency)
{
    NanScope();
#ifdef HTS_ENGINE_DEBUG
    fprintf(stderr, "HTSEngineNode::GetSamplingFrequency\n");
#endif
    HTSEngineNode *context = ObjectWrap::Unwrap<HTSEngineNode>(args.This());
    NanReturnValue(Int32::New(context->samplingFrequency));
};

NAN_SETTER(HTSEngineNode::SetSamplingFrequency)
{
    NanScope();
#ifdef HTS_ENGINE_DEBUG
    fprintf(stderr, "HTSEngineNode::SetSamplingFrequency\n");
#endif
    if (value->IsInt32())
    {
        HTSEngineNode *context = ObjectWrap::Unwrap<HTSEngineNode>(args.This());
        context->samplingFrequency = value->ToInt32()->Value();
    } else {
        NanThrowError("samplingFrequency is Int32.");
    }
};
// framePeriod getter setter
NAN_GETTER(HTSEngineNode::GetFramePeriod)
{
    NanScope();
#ifdef HTS_ENGINE_DEBUG
    fprintf(stderr, "HTSEngineNode::GetFramePeriod\n");
#endif
    HTSEngineNode *context = ObjectWrap::Unwrap<HTSEngineNode>(args.This());
    NanReturnValue(Int32::New(context->framePeriod));
};
NAN_SETTER(HTSEngineNode::SetFramePeriod)
{
    NanScope();
#ifdef HTS_ENGINE_DEBUG
    fprintf(stderr, "HTSEngineNode::SetFramePeriod\n");
#endif
    if (value->IsInt32())
    {
        HTSEngineNode *context = ObjectWrap::Unwrap<HTSEngineNode>(args.This());
        context->framePeriod = value->ToInt32()->Value();
    } else {
        NanThrowError("framePeriod is Int32.");
    }
};
// allPassConstant getter setter
NAN_GETTER(HTSEngineNode::GetAllPassConstant)
{
    NanScope();
#ifdef HTS_ENGINE_DEBUG
    fprintf(stderr, "HTSEngineNode::GetAllPassConstant\n");
#endif
    HTSEngineNode *context = ObjectWrap::Unwrap<HTSEngineNode>(args.This());
    NanReturnValue(Number::New(context->allPassConstant));
};
NAN_SETTER(HTSEngineNode::SetAllPassConstant)
{
    NanScope();
#ifdef HTS_ENGINE_DEBUG
    fprintf(stderr, "HTSEngineNode::SetAllPassConstant\n");
#endif
    if (value->IsNumber())
    {
        HTSEngineNode *context = ObjectWrap::Unwrap<HTSEngineNode>(args.This());
        context->allPassConstant = value->ToNumber()->Value();
    } else {
        NanThrowError("allPassConstant is Number.");
    }
};
// postFilteringCoefficient getter setter
NAN_GETTER(HTSEngineNode::GetPostFilteringCoefficient)
{
    NanScope();
#ifdef HTS_ENGINE_DEBUG
    fprintf(stderr, "HTSEngineNode::GetPostFilteringCoefficient\n");
#endif
    HTSEngineNode *context = ObjectWrap::Unwrap<HTSEngineNode>(args.This());
    NanReturnValue(Number::New(context->postFilteringCoefficient));
};
NAN_SETTER(HTSEngineNode::SetPostFilteringCoefficient)
{
    NanScope();
#ifdef HTS_ENGINE_DEBUG
    fprintf(stderr, "HTSEngineNode::SetPostFilteringCoefficient\n");
#endif
    if (value->IsNumber())
    {
        HTSEngineNode *context = ObjectWrap::Unwrap<HTSEngineNode>(args.This());
        context->postFilteringCoefficient = value->ToNumber()->Value();
    } else {
        NanThrowError("postFilteringCoefficient is Number.");
    }
};
// speechSpeedRate getter setter
NAN_GETTER(HTSEngineNode::GetSpeechSpeedRate)
{
    NanScope();
#ifdef HTS_ENGINE_DEBUG
    fprintf(stderr, "HTSEngineNode::GetSpeechSpeedRate\n");
#endif
    HTSEngineNode *context = ObjectWrap::Unwrap<HTSEngineNode>(args.This());
    NanReturnValue(Number::New(context->speechSpeedRate));
};
NAN_SETTER(HTSEngineNode::SetSpeechSpeedRate)
{
    NanScope();
#ifdef HTS_ENGINE_DEBUG
    fprintf(stderr, "HTSEngineNode::SetSpeechSpeedRate\n");
#endif
    if (value->IsNumber())
    {
        HTSEngineNode *context = ObjectWrap::Unwrap<HTSEngineNode>(args.This());
        context->speechSpeedRate = value->ToNumber()->Value();
    } else {
        NanThrowError("speechSpeedRate is Number.");
    }
};
// additionalHalfTone getter setter
NAN_GETTER(HTSEngineNode::GetAdditionalHalfTone)
{
    NanScope();
#ifdef HTS_ENGINE_DEBUG
    fprintf(stderr, "HTSEngineNode::GetAdditionalHalfTone\n");
#endif
    HTSEngineNode *context = ObjectWrap::Unwrap<HTSEngineNode>(args.This());
    NanReturnValue(Number::New(context->additionalHalfTone));
};
NAN_SETTER(HTSEngineNode::SetAdditionalHalfTone)
{
    NanScope();
#ifdef HTS_ENGINE_DEBUG
    fprintf(stderr, "HTSEngineNode::SetAdditionalHalfTone\n");
#endif
    if (value->IsNumber())
    {
        HTSEngineNode *context = ObjectWrap::Unwrap<HTSEngineNode>(args.This());
        context->additionalHalfTone = value->ToNumber()->Value();
    } else {
        NanThrowError("additionalHalfTone is Number.");
    }
};
// unvoicedThreshold getter setter
NAN_GETTER(HTSEngineNode::GetUnvoicedThreshold)
{
    NanScope();
#ifdef HTS_ENGINE_DEBUG
    fprintf(stderr, "HTSEngineNode::GetUnvoicedThreshold\n");
#endif
    HTSEngineNode *context = ObjectWrap::Unwrap<HTSEngineNode>(args.This());
    NanReturnValue(Number::New(context->unvoicedThreshold));
};
NAN_SETTER(HTSEngineNode::SetUnvoicedThreshold)
{
    NanScope();
#ifdef HTS_ENGINE_DEBUG
    fprintf(stderr, "HTSEngineNode::SetUnvoicedThreshold\n");
#endif
    if (value->IsNumber())
    {
        HTSEngineNode *context = ObjectWrap::Unwrap<HTSEngineNode>(args.This());
        context->unvoicedThreshold = value->ToNumber()->Value();
    } else {
        NanThrowError("unvoicedThreshold is Number.");
    }
};
// interpolation getter setter
NAN_GETTER(HTSEngineNode::GetInterpolation)
{
    NanScope();
#ifdef HTS_ENGINE_DEBUG
    fprintf(stderr, "HTSEngineNode::GetInterpolation\n");
#endif
    HTSEngineNode *context = ObjectWrap::Unwrap<HTSEngineNode>(args.This());
    Local<Object> interpolationObject = Object::New();
    for (std::map<std::string, double>::iterator it = context->interpolation.begin(); it != context->interpolation.end(); ++it)
    {
        interpolationObject->Set(NanSymbol(it->first.c_str()), Number::New(it->second));
    }
    NanReturnValue(interpolationObject);
};
NAN_SETTER(HTSEngineNode::SetInterpolation)
{
    NanScope();
#ifdef HTS_ENGINE_DEBUG
    fprintf(stderr, "HTSEngineNode::SetInterpolation\n");
#endif
    HTSEngineNode *context = ObjectWrap::Unwrap<HTSEngineNode>(args.This());
    Local<Object> interpolationObject = Object::New();
    if (value->IsObject())
    {
        Local<Object> interpolationObject = value->ToObject();
        Local<Array> interpolationKeysArray = interpolationObject->GetPropertyNames();
        for (uint32_t i = 0; i < interpolationKeysArray->Length(); i++)
        {
            Handle<String> keyString = interpolationKeysArray->Get(Integer::New(i))->ToString();
            Handle<Number> valueNumber = interpolationObject->Get(keyString)->ToNumber();
            v8::String::Utf8Value interpolationKey(keyString->ToString());
            std::string key = std::string(*interpolationKey);
            fprintf(stderr, "interpolation key%s\n", key.c_str());
            std::map<std::string, double>::iterator ret = context->interpolation.find(key.c_str());
            //swap
            if (ret != context->interpolation.end())
            {
                context->interpolation[ret->first] = valueNumber->Value();
            }
        }
    } else {
        NanThrowError("interpolation is Object.");
    }
};

/**
 * [Init description]
 * @param exports [description]
 */
void InitNodeModule(Handle<Object> exports)
{
    HTSEngineNode::Init();
#ifdef HTS_ENGINE_DEBUG
    fprintf(stderr, "hts_engine_api::InitNodeModule!\n");
#endif
    Local<Function> HTSEngine = FunctionTemplate::New(HTSEngineNode::NewInstance)->GetFunction();
    exports->Set(NanSymbol("HTSEngine"), HTSEngine);
}

NODE_MODULE(hts_engine_api, InitNodeModule);