#!/usr/bin/env node

/*
 * Types = command, core, event, lib, message
 */

const fs = require("fs");

const yikes = (...a) => { console.error(...a); process.exit(1); };

const [,, ...args] = process.argv;

if ((args[0] || "").match(/^(types|help)$/i)) {
	console.log("Supported types:", ["command", "core", "event", "lib", "message"].map(x => `\x1b[1m${x}\x1b[0m`).join(", "));
	process.exit(0);
}

if (args.length < 2 || !args[1]) {
	yikes("Usage: new.js [type] [name]");
}

const [type, name] = args;

if (!name.match(/^[\w_\d]+$/i)) {
	yikes("Invalid name:", name);
}

let allDir, sourcename, headername, sourcetext, headertext;

const sourcebase = "src", headerbase = "include";

if (type.match(/^(com(mands?)?|cmd)$/i)) {

	sourcename = `commands/${name}.cpp`;
	headername = `commands/${name}.h`;
	const cls = `${name}_command`;

	headertext = prepare(`
	%	#ifndef PINGPONG_COMMANDS_${name.toUpperCase()}_H_
	%	#define PINGPONG_COMMANDS_${name.toUpperCase()}_H_
	%	
	%	#include "commands/command.h"
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
	%	#include "commands/${name}.h"
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
	%	#include "messages/message.h"
	%	${base? `#include "${base}.h"\n` : ""}
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
	%	#include "messages/${name}.h"
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
			ircText = ircText.replace(/(#include "events\/server_status.h"\n)/, `$1\n#include "messages/${name}.h"`));
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
	%	#include "events/event.h"
	%	${base? `#include "events/${base}.h"\n` : ""}
	%	namespace pingpong {
	%		class ${cls}: public ${parent} {
	%			public:
	%				using ${parent}::${parent};
	%		};
	%	}
	%	
	%	#endif`);

	allDir = "events";

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
	%	#include "lib/${name}.h"
	%	
	%	namespace pingpong {
	%		
	%	}`);

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
	%	#include "core/${name}.h"
	%
	%	namespace pingpong {
	%		
	%	}`);

} else {
	console.error("Unknown type:", type);
	yikes(`Expected "command" | "core" | "lib" | "message" | "event"`);
}

if (sourcetext) {
	const fn = `${sourcebase}/${sourcename}`;
	if (fs.existsSync(fn))
		return console.error(`Error: \x1b[1m${fn}\x1b[0m already exists.`);
	fs.writeFileSync(fn, sourcetext);
}

if (headertext) {
	const fn = `${headerbase}/${headername}`;
	if (fs.existsSync(fn))
		return console.error(`Error: \x1b[1m${fn}\x1b[0m already exists.`);
	fs.writeFileSync(fn, headertext);
}

if (allDir) {
	const allText = fs.readFileSync(`${headerbase}/${allDir}/all.h`, "utf8");
	if (allText.indexOf(`#include "${name}.h"`) == -1) {
		fs.writeFileSync(`${headerbase}/${allDir}/all.h`, allText.replace(/(\n#endif)/, `#include "${name}.h"\n$1`));
	}
}

function prepare(text) {
	// Remove the initial newline and then the extra tabs and add a trailing newline..
	return text.substr(1).replace(/^\t+%\t?/gm, "") + "\n";
}
