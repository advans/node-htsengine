{
  "variables": {
    "htsengine_dir": "<(module_root_dir)/lib/htsengineapi",
    "cflags": "-fexceptions -fPIC",
    "cflags!": "-fno-exception -fno-rtti",
  },
  "targets": [{
    "target_name": "hts_engine_api",
    'configurations': {
      'Debug': {
        'defines': ['HTS_ENGINE_DEBUG', '_DEBUG']
      }
    },
    "include_dirs": [
      "<!(node -e \"require('nan')\")",
      "<(htsengine_dir)/include",
      "<(htsengine_dir)/lib"
    ],
    "sources": [
      "src/hts_engine_api.cc",
      "src/hts_engine_wave.cc",
      "src/hts_engine_file_async.cc",
      "src/hts_engine_string_async.cc",
      "lib/htsengineapi/lib/HTS_audio.c",
      "lib/htsengineapi/lib/HTS_engine.c",
      "lib/htsengineapi/lib/HTS_gstream.c",
      "lib/htsengineapi/lib/HTS_label.c",
      "lib/htsengineapi/lib/HTS_misc.c",
      "lib/htsengineapi/lib/HTS_model.c",
      "lib/htsengineapi/lib/HTS_pstream.c",
      "lib/htsengineapi/lib/HTS_sstream.c",
      "lib/htsengineapi/lib/HTS_vocoder.c",
    ],
  }]
}