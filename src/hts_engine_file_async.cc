#include <node.h>
#include "nan.h"
#include <stdlib.h>
#include "hts_engine_api.h"

using namespace v8;

HTSEngineNodeFileAsyncWorker::HTSEngineNodeFileAsyncWorker(NanCallback *callback, HTS_Engine _engine, char *_fileName): NanAsyncWorker(callback)
{
#ifdef HTS_ENGINE_DEBUG
    fprintf(stderr, "HTSEngineNodeFileAsyncWorker::HTSEngineNodeFileAsyncWorker\n");
#endif
    engine = _engine;
    fileName = const_cast<char *>(_fileName);
};
HTSEngineNodeFileAsyncWorker::~HTSEngineNodeFileAsyncWorker()
{
#ifdef HTS_ENGINE_DEBUG
    fprintf(stderr, "HTSEngineNodeFileAsyncWorker::~HTSEngineNodeFileAsyncWorker\n");
#endif
    // auto GC ?
    HTS_Engine_clear(&engine);
    //
    free(fileName);
};


void HTSEngineNodeFileAsyncWorker::Execute()
{
#ifdef HTS_ENGINE_DEBUG
    fprintf(stderr, "HTSEngineNodeFileAsyncWorker::Execute\n");
#endif
    /* synthesize */
    if (HTS_Engine_synthesize_from_fn(&engine, fileName) != TRUE)
    {
        errmsg = strdup("HTS labelFile cannot be synthesized");
        return;
    }
}

void HTSEngineNodeFileAsyncWorker::HandleOKCallback ()
{
    NanScope();
#ifdef HTS_ENGINE_DEBUG
    printf("%s\n", "HTSEngineNodeFileAsyncWorker::HandleOKCallback" );
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