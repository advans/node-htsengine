var assert = require("assert");
var fs = require('fs');
var path = require('path');

var DEFAULT_VOICE_CMU = path.join(__dirname, '../voice/cmu_us_arctic_slt.htsvoice');
var DEFAULT_VOICE_MEI = path.join(__dirname, '../voice/mei_bashful.htsvoice');
var DEFAULT_VOICE_MEI2 = path.join(__dirname, '../voice/mei_normal.htsvoice');

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
	describe('hts_engine_api.node', function() {
		var _module;
		var engine;
		before(function(done) {
			if (fs.existsSync(path.join(__dirname, '../build/Debug'))) {
				_module = require('../build/Debug/hts_engine_api');
			} else if (fs.existsSync(path.join(__dirname, '../build/Release'))) {
				_module = require('../build/Release/hts_engine_api');
			}
			engine = new _module.HTSEngine();
			done();
		});
		it('htsvoice', function() {
			assert.equal(engine.htsvoice, undefined);
			assert.throws(function() {
				engine.htsvoice = "TEST";
			});
			engine.htsvoice = DEFAULT_VOICE_CMU;
			assert.equal(engine.htsvoice, DEFAULT_VOICE_CMU);
			engine.htsvoice = [DEFAULT_VOICE_MEI, DEFAULT_VOICE_MEI2];
			assert.equal(engine.htsvoice.length, 2);
			engine.htsvoice = [{
				file: DEFAULT_VOICE_MEI,
				interpolation: 2
			}, {
				file: DEFAULT_VOICE_MEI2
			}];
			assert.equal(engine.interpolation[DEFAULT_VOICE_MEI], 2);
		});
		it('phonemeAlignment', function() {
			assert.equal(engine.phonemeAlignment, false);
			engine.phonemeAlignment = 1;
			assert.equal(engine.phonemeAlignment, false);
			engine.phonemeAlignment = "1";
			assert.equal(engine.phonemeAlignment, false);
			engine.phonemeAlignment = true;
			assert.equal(engine.phonemeAlignment, true);
		});
	})
});