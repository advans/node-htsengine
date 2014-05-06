# node-htsengine
[![Build Status](https://travis-ci.org/advans/node-htsengine.svg?branch=master)](https://travis-ci.org/advans/node-htsengine)

node.js for [hts_engine API](http://hts-engine.sourceforge.net/)

## Install

```bash
npm install htsengine
```

## Example
```js
var fs = require('fs');
var htsengine = require('htsengine');

var engine = new htsengine.engine({
    "htsvoice": htsengine.voice['en']['default']
});
// from Label File
var wavBuffer = engine.synthesizeFormFileSync(labelFile);
fs.writeFileSync(WavFile.wav, wavBuffer);
// async
engine.synthesizeFormFile(labelFile, function(err, wavBuffer) {
        if (!err) {
            fs.writeFileSync(WavFile.wav, wavBuffer);
        }
    }
});
fs.writeFileSync(WavFile.wav, wavBuffer);
// from Label Text
var wavBuffer = engine.synthesizeFormStringSync([labelText1, labelText2, labelText3, ...]);
fs.writeFileSync(WavFile.wav, wavBuffer);
// async
engine.synthesizeFormString([labelText1, labelText2, labelText3, ...], function(err, wavBuffer) {
        if (!err) {
            fs.writeFileSync(WavFile.wav, wavBuffer);
        }
    }
});

```

## Engine options
```js
// htsvoice   : HTS voice files
htsengine.htsvoice = "XXXX.htsvoice";
// to Array
htsengine.htsvoice = ["XXXX.htsvoice"];
// to Object
htsengine.htsvoice = [{
    "file": "XXXX.htsvoice"
}, {
    "file": "YYYY.htsvoice"
}];

// use phoneme alignment for duration
htsengine.phonemeAlignment = true;

// sampling frequency
htsengine.samplingFrequency = 48000;

// frame period (point)
htsengine.framePeriod = 240;

// all-pass constant
htsengine.allPassConstant = 0.55;

// postfiltering coefficient
htsengine.postFilteringCoefficient = 0;

// speech speed rate
htsengine.speechSpeedRate = 1;

// additional half-tone
htsengine.additionalHalfTone = 0;

// voiced/unvoiced threshold
htsengine.unvoicedThreshold = 0.5;

// enable interpolation
htsengine.htsvoice = [{
    "file": "XXXX.htsvoice",
    "interpolation": 1
}, {
    "file": "YYYY.htsvoice",
    "interpolation": 2
}];
```