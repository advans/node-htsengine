var assert = require("assert");
var fs = require('fs');
var path = require('path');

var DEFAULT_VOICE_CMU = path.join(__dirname, '../voice/cmu_us_arctic_slt.htsvoice');

describe('load module TEST', function() {
	describe('load hts_engine_api.node', function() {
		it('has been compiled', function() {
			var HTSEngine;
			if (fs.existsSync(path.join(__dirname, '../build/Debug'))) {
				HTSEngine = require('../build/Debug/hts_engine_api');
			} else if (fs.existsSync(path.join(__dirname, '../build/Release'))) {
				HTSEngine = require('../build/Release/hts_engine_api');
			} else {
				throw '"HTSEngine" has not been compiled yet.'
			}
			assert.ok(HTSEngine.hasOwnProperty("HTSEngine"));
			//assert.equal(-1, [1, 2, 3].indexOf(0));
		})
	})
});

describe('Prototype TEST', function() {
	var _module;
	var engine;
	if (fs.existsSync(path.join(__dirname, '../build/Debug'))) {
		_module = require('../build/Debug/hts_engine_api');
	} else if (fs.existsSync(path.join(__dirname, '../build/Release'))) {
		_module = require('../build/Release/hts_engine_api');
	}
	engine = new _module.HTSEngine();
	describe('engine.htsvoice', function() {
		it('Default engine.htsvoice = "undefined"', function() {
			assert.equal(engine.htsvoice, undefined);
		});
		it('engine.htsvoice = "unsupported file"', function() {
			assert.throws(function() {
				engine.htsvoice = "TEST";
			});
		});
		it('engine.htsvoice = "fileName<String>"', function() {
			engine.htsvoice = DEFAULT_VOICE_CMU;
			assert.equal(engine.htsvoice, DEFAULT_VOICE_CMU);
		});
		it('engine.htsvoice = "[fileName<String>,...]"', function() {
			engine.htsvoice = [DEFAULT_VOICE_CMU, DEFAULT_VOICE_CMU];
			assert.equal(engine.htsvoice.length, 2);
		});
		it('engine.htsvoice = "[voiceConfig<Object>,...]"', function() {
			engine.htsvoice = [{
				file: DEFAULT_VOICE_CMU,
				interpolation: 2
			}, {
				file: DEFAULT_VOICE_CMU
			}];
			assert.equal(engine.interpolation[DEFAULT_VOICE_CMU], 2);
		});
	});
	describe('engine.phonemeAlignment<Boolean>', function() {
		it('Default engine.phonemeAlignment = false', function() {
			assert.equal(engine.phonemeAlignment, false);
		});
		it('engine.phonemeAlignment = Number', function() {
			assert.throws(function() {
				engine.phonemeAlignment = 1;
			});
		});
		it('engine.phonemeAlignment = String', function() {
			assert.throws(function() {
				engine.phonemeAlignment = "1";
			});
		});
		it('engine.phonemeAlignment = Boolean', function() {
			engine.phonemeAlignment = true;
			assert.equal(engine.phonemeAlignment, true);
		});
	});
	//framePeriod
	describe('engine.framePeriod<Number(Int32)>', function() {
		it('Default engine.framePeriod = 240', function() {
			assert.equal(engine.framePeriod, 240);
		});
		it('engine.framePeriod = Number(Float)', function() {
			assert.throws(function() {
				engine.framePeriod = 0.1
			});
		});
		it('engine.framePeriod = String', function() {
			assert.throws(function() {
				engine.framePeriod = "1";
			});
		});
		it('engine.framePeriod = Boolean', function() {
			assert.throws(function() {
				engine.framePeriod = true;
			});
		});
		it('engine.framePeriod = Number(Int32)', function() {
			engine.framePeriod = 480;
			assert.equal(engine.framePeriod, 480);
		});
	});
	//allPassConstant
	describe('engine.allPassConstant<Number(Float)>', function() {
		it('Default engine.allPassConstant = 0.55', function() {
			assert.equal(engine.allPassConstant, 0.55);
		});
		it('engine.allPassConstant = Number(Int32)', function() {
			engine.allPassConstant = 1;
			assert.equal(engine.allPassConstant, 1);
		});
		it('engine.allPassConstant = String', function() {
			assert.throws(function() {
				engine.allPassConstant = "1";
			});
		});
		it('engine.allPassConstant = Boolean', function() {
			assert.throws(function() {
				engine.allPassConstant = true;
			});
		});
		it('engine.allPassConstant = Number(Float)', function() {
			engine.allPassConstant = 0.5;
			assert.equal(engine.allPassConstant, 0.5);
		});
	});
	//postFilteringCoefficient
	describe('engine.postFilteringCoefficient<Number(Float)>', function() {
		it('Default engine.postFilteringCoefficient = 0', function() {
			assert.equal(engine.postFilteringCoefficient, 0);
		});
		it('engine.postFilteringCoefficient = Number(Int32)', function() {
			engine.postFilteringCoefficient = 1;
			assert.equal(engine.postFilteringCoefficient, 1);
		});
		it('engine.postFilteringCoefficient = String', function() {
			assert.throws(function() {
				engine.postFilteringCoefficient = "1";
			});
		});
		it('engine.postFilteringCoefficient = Boolean', function() {
			assert.throws(function() {
				engine.postFilteringCoefficient = true;
			});
		});
		it('engine.postFilteringCoefficient = Number(Float)', function() {
			engine.postFilteringCoefficient = 0.5;
			assert.equal(engine.postFilteringCoefficient, 0.5);
		});
	});
	//speechSpeedRate
	describe('engine.speechSpeedRate<Number(Float)>', function() {
		it('Default engine.speechSpeedRate = 1', function() {
			assert.equal(engine.speechSpeedRate, 1);
		});
		it('engine.speechSpeedRate = Number(Int32)', function() {
			engine.speechSpeedRate = 1;
			assert.equal(engine.speechSpeedRate, 1);
		});
		it('engine.speechSpeedRate = String', function() {
			assert.throws(function() {
				engine.speechSpeedRate = "1";
			});
		});
		it('engine.speechSpeedRate = Boolean', function() {
			assert.throws(function() {
				engine.speechSpeedRate = true;
			});
		});
		it('engine.speechSpeedRate = Number(Float)', function() {
			engine.speechSpeedRate = 0.5;
			assert.equal(engine.speechSpeedRate, 0.5);
		});
	});
	//additionalHalfTone
	describe('engine.additionalHalfTone<Number(Float)>', function() {
		it('Default engine.additionalHalfTone = 0', function() {
			assert.equal(engine.additionalHalfTone, 0);
		});
		it('engine.additionalHalfTone = Number(Int32)', function() {
			engine.additionalHalfTone = 1;
			assert.equal(engine.additionalHalfTone, 1);
		});
		it('engine.additionalHalfTone = String', function() {
			assert.throws(function() {
				engine.additionalHalfTone = "1";
			});
		});
		it('engine.additionalHalfTone = Boolean', function() {
			assert.throws(function() {
				engine.additionalHalfTone = true;
			});
		});
		it('engine.additionalHalfTone = Number(Float)', function() {
			engine.additionalHalfTone = 0.5;
			assert.equal(engine.additionalHalfTone, 0.5);
		});
	});
	//unvoicedThreshold
	describe('engine.unvoicedThreshold<Number(Float)>', function() {
		it('Default engine.unvoicedThreshold = 0.5', function() {
			assert.equal(engine.unvoicedThreshold, 0.5);
		});
		it('engine.unvoicedThreshold = Number(Int32)', function() {
			engine.unvoicedThreshold = 1;
			assert.equal(engine.unvoicedThreshold, 1);
		});
		it('engine.unvoicedThreshold = String', function() {
			assert.throws(function() {
				engine.unvoicedThreshold = "1";
			});
		});
		it('engine.unvoicedThreshold = Boolean', function() {
			assert.throws(function() {
				engine.unvoicedThreshold = true;
			});
		});
		it('engine.unvoicedThreshold = Number(Float)', function() {
			engine.unvoicedThreshold = 0.5;
			assert.equal(engine.unvoicedThreshold, 0.5);
		});
	});
});