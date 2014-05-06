#include <node.h>
#include "nan.h"
#include <stdlib.h>
#include "hts_engine_api.h"

using namespace v8;

HTSEngineNodeStringAsyncWorker::HTSEngineNodeStringAsyncWorker(NanCallback *callback, HTS_Engine _engine, Handle<Array> _labelArray): NanAsyncWorker(callback)
{
#ifdef HTS_ENGINE_DEBUG
    fprintf(stderr, "HTSEngineNodeStringAsyncWorker::HTSEngineNodeStringAsyncWorker\n");
#endif
    engine = _engine;
    labelArray = _labelArray;
    num_lines = labelArray->Length();
    lines = (char **) malloc(num_lines * sizeof(char *));
    for (uint32_t i = 0; i < num_lines; i++)
    {
        size_t count;
        Handle<String> labelString = labelArray->Get(Integer::New(i))->ToString();
        lines[i] = NanCString(labelString, &count);
    }
};

HTSEngineNodeStringAsyncWorker::~HTSEngineNodeStringAsyncWorker()
{
#ifdef HTS_ENGINE_DEBUG
    fprintf(stderr, "HTSEngineNodeStringAsyncWorker::~HTSEngineNodeStringAsyncWorker\n");
#endif
    // auto GC ?
    HTS_Engine_clear(&engine);
    free(lines);
};

void HTSEngineNodeStringAsyncWorker::Execute()
{
#ifdef HTS_ENGINE_DEBUG
    fprintf(stderr, "HTSEngineNodeStringAsyncWorker::Execute\n");
#endif

    /* synthesize */
    if (HTS_Engine_synthesize_from_strings(&engine, lines, num_lines) != TRUE)
    {
        errmsg = strdup("HTS labelFile cannot be synthesized");
        return;
    }
};

void HTSEngineNodeStringAsyncWorker::HandleOKCallback ()
{
    NanScope();
#ifdef HTS_ENGINE_DEBUG
    printf("%s\n", "HTSEngineNodeStringAsyncWorker::HandleOKCallback" );
#endif

    HTSEngineNodeWave *wav = new HTSEngineNodeWave();
    Local<Object> wavFileBuffer = wav->create(&engine);
    free(wav);
    Local<Value> argv[] =
    {
        NanNewLocal<Value>(Null()),
        wavFileBuffer
    };
    callback->Call(2, argv);
};