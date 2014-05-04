var fs = require('fs');
var path = require('path');
var _module;
// load module
if (fs.existsSync(path.join(__dirname, './build/Debug'))) {
	_module = require('./build/Debug/hts_engine_api');
} else if (fs.existsSync(path.join(__dirname, './build/Release'))) {
	_module = require('./build/Release/hts_engine_api');
} else {
	throw '"HTSEngine" has not been compiled yet.'
}

// default voice set
var en = {
	'default': path.join(__dirname, '/voice/cmu_us_arctic_slt.htsvoice')
};
var voice = {
	'en': en,
	'ja': ja
};

module.exports.voices = voice;
module.exports.engine = _module.HTSEngine;