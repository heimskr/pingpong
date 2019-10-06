#!/usr/bin/env node

/*
 * Types = command, core, event, lib, message, numeric
 */

const fs = require("fs"),
      minimist = require("minimist");

const yikes = (...a) => { console.error(...a); process.exit(1); };

const [,, ...args] = process.argv;

const options = minimist(process.argv.slice(2), {
	alias: {
		s: "nosrc",
		h: "noheader",
		o: "overwrite"
	},
	boolean: ["nosrc", "noheader", "overwrite"],
	default: {
		nosrc: false,
		noheader: false,
		overwrite: false,
	}
});

if ((args[0] || "").match(/^(types|help)$/i)) {
	console.log("Supported types:", ["command", "core", "event", "lib", "message", "numeric"].map(x => `\x1b[1m${x}\x1b[0m`).join(", "));
	process.exit(0);
}

if (args.length < 2 || !args[1]) {
	yikes("Usage: new.js [type=command,core,event,lib,message,numeric] [name] [-s/--nosrc] [-h/--noheader] [-o/--overwrite]");
}

const [type, name] = args;

if (!name.match(/^[\w_\d]+$/i)) {
	yikes("Invalid name:", name);
}

let sourcename, headername, sourcetext, headertext;
let afterWrite = () => {};

const sourcebase = "src", headerbase = "include/pingpong";

if (type.match(/^(com(mands?)?|cmd)$/i)) {

	sourcename = `commands/${name}.cpp`;
	headername = `commands/${name}.h`;
	const cls = `${name}_command`;

	headertext = prepare(`
	%	#ifndef PINGPONG_COMMANDS_${name.toUpperCase()}_H_
	%	#define PINGPONG_COMMANDS_${name.toUpperCase()}_H_
	%	
	%	#include "pingpong/commands/command.h"
	%	
	%	namespace pingpong {
	%		class ${cls}: public command {
	%			public:
	%				
	%				operator std::string() const override;
	%				virtual void send() override;
	%		};
	%	}
	%	
	%	#endif`);

	sourcetext = prepare(`
	%	#include "pingpong/commands/${name}.h"
	%	
	%	namespace pingpong {
	%		${cls}::operator std::string() const {
	%			
	%		}
	%	}`);

} else if (type.match(/^(mes(sages?)?|msg|m)(-\w+)?$/i)) {

	const base = type.match(/-/)? type.replace(/^.+-/, "") : null;
	const parent = base? `${base}_message` : "message";

	sourcename = `messages/${name}.cpp`;
	headername = `messages/${name}.h`;
	const cls = `${name}_message`;

	headertext = prepare(`
	%	#ifndef PINGPONG_MESSAGES_${name.toUpperCase()}_H_
	%	#define PINGPONG_MESSAGES_${name.toUpperCase()}_H_
	%	
	%	#include "pingpong/messages/message.h"
	%	${base? `#include "pingpong/messages/${base}.h"\n` : ""}
	%	namespace pingpong {
	%		class ${cls}: public ${parent} {
	%			public:
	%				using ${parent}::${parent};
	%				static constexpr auto get_name = []() -> std::string { return "${args[2] || name.toUpperCase()}"; };
	%	
	%				operator std::string() const override;
	%		};
	%	}
	%	
	%	#endif`);

	sourcetext = prepare(`
	%	#include "pingpong/messages/${name}.h"
	%	
	%	namespace pingpong {
	%		${cls}::operator std::string() const {
	%			
	%		}
	%	}`);

	const ircPath = sourcebase + "/core/irc.cpp";
	let ircText = fs.readFileSync(ircPath, "utf8");
	if (ircText.indexOf(`add_ctor<${cls}>`) == -1) {
		fs.writeFileSync(ircPath,
			ircText = ircText.replace(/(void irc::init_messages\(\) {)/, `$1\n\t\tmessage::add_ctor<${cls}>();`));
	}

	if (ircText.indexOf(`"messages/${name}.h"`) == -1) {
		fs.writeFileSync(ircPath,
			ircText = ircText.replace(/(#include "pingpong\/events\/server_status.h"\n)/, `$1\n#include "pingpong/messages/${name}.h"`));
	}

} else if (type.match(/^(ev?|ev(en)?t)(-\w+)?$/i)) {

	const base = type.match(/-/)? type.replace(/^.+-/, "") : null;
	const parent = base? `${base}_event` : "event";

	headername = `events/${name}.h`;
	const cls = `${name}_event`;

	headertext = prepare(`
	%	#ifndef PINGPONG_EVENTS_${name.toUpperCase()}_H_
	%	#define PINGPONG_EVENTS_${name.toUpperCase()}_H_
	%	
	%	#include "pingpong/events/${["sourced", "targeted"].includes(base)? base : "event"}.h"
	%	
	%	namespace pingpong {
	%		struct ${cls}: public ${parent} {
	%			using ${parent}::${parent};
	%		};
	%	}
	%	
	%	#endif`);

} else if (type.match(/^l(ib(rary)?)?$/i)) {

	sourcename = `lib/${name}.cpp`;
	headername = `lib/${name}.h`;

	headertext = prepare(`
	%	#ifndef PINGPONG_LIB_${name.toUpperCase()}_H_
	%	#define PINGPONG_LIB_${name.toUpperCase()}_H_
	%	
	%	namespace pingpong {
	%		
	%	}
	%	
	%	#endif`);

	sourcetext = prepare(`
	%	#include "pingpong/lib/${name}.h"
	%	
	%	namespace pingpong {
	%		
	%	}`);

} else if (type.match(/^n(um(eric)?)?$/i)) {

	sourcename = `numerics/${name}.cpp`;

	sourcetext = prepare(`
	%	#include "pingpong/messages/numeric.h"
	%
	%	namespace pingpong {
	%		bool numeric_message::handle_${name}(server *serv) {
	%			
	%			return true;
	%		}
	%	}`);

	afterWrite = () => {
		let numeric_h   = fs.readFileSync("include/pingpong/messages/numeric.h", "utf8");
		let numeric_cpp = fs.readFileSync("src/messages/numeric.cpp",   "utf8");

		numeric_h = numeric_h.replace(/(\n\t};\n}\n\n#endif)/, `\n\t\t\tbool handle_${name}(server *);$1`);
		numeric_cpp = numeric_cpp.replace(/(default: return true;)/, `case numeric_type::${name}: return handle_${name}(serv);\n\t\t\t$1`);

		fs.writeFileSync("include/pingpong/messages/numeric.h", numeric_h);
		fs.writeFileSync("src/messages/numeric.cpp", numeric_cpp);
	};

} else if (type.match(/^core$/i)) {

	sourcename = `core/${name}.cpp`;
	headername = `core/${name}.h`;

	headertext = prepare(`
	%	#ifndef PINGPONG_CORE_${name.toUpperCase()}_H_
	%	#define PINGPONG_CORE_${name.toUpperCase()}_H_
	%	
	%	namespace pingpong {
	%		
	%	}
	%	
	%	#endif`);

	sourcetext = prepare(`
	%	#include "pingpong/core/${name}.h"
	%
	%	namespace pingpong {
	%		
	%	}`);

} else {
	console.error("Unknown type:", type);
	yikes(`Expected "command" | "core" | "lib" | "message" | "event"`);
}

if (sourcetext && !options.nosrc) {
	const fn = `${sourcebase}/${sourcename}`;
	if (fs.existsSync(fn) && !options.overwrite) {
		console.error(`Error: \x1b[1m${fn}\x1b[0m already exists.`);
	} else {
		fs.writeFileSync(fn, sourcetext);
		console.log(`Wrote to \x1b[1m${fn}\x1b[0m.`);
	}
}

if (headertext && !options.noheader) {
	const fn = `${headerbase}/${headername}`;
	if (fs.existsSync(fn) && !options.overwrite) {
		console.error(`Error: \x1b[1m${fn}\x1b[0m already exists.`);
	} else {
		fs.writeFileSync(fn, headertext);
		console.log(`Wrote to \x1b[1m${fn}\x1b[0m.`);
	}
}

afterWrite();

function prepare(text) {
	// Remove the initial newline and then the extra tabs and add a trailing newline..
	return text.substr(1).replace(/^\t+%\t?/gm, "") + "\n";
}
